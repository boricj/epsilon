#include "calculation_controller.h"
#include "apps/constant.h"
#include "apps/apps_container.h"
#include "app.h"
#include <poincare.h>
#include <assert.h>

using namespace Shared;
using namespace Poincare;

namespace Statistics {

CalculationController::CalculationController(Responder * parentResponder, ButtonRowController * header, Store * store) :
  TabTableController(parentResponder, this),
  ButtonRowDelegate(header, nullptr),
  m_titleCells{},
  m_calculationCells{},
  m_store(store)
{
}

const char * CalculationController::title() {
  return I18n::translate(&I18n::Common::StatTab);
}

bool CalculationController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::Up) {
    selectableTableView()->deselectTable();
    app()->setFirstResponder(tabController());
    return true;
  }
  if (event == Ion::Events::Copy && selectedColumn() == 1) {
    EvenOddBufferTextCell * myCell = (EvenOddBufferTextCell *)selectableTableView()->selectedCell();
    Clipboard::sharedClipboard()->store(myCell->text());
    return true;
  }
  return false;
}

void CalculationController::didBecomeFirstResponder() {
  if (selectedRow() == -1) {
    selectCellAtLocation(0, 0);
  } else {
    selectCellAtLocation(selectedColumn(), selectedRow());
  }
  TabTableController::didBecomeFirstResponder();
}

bool CalculationController::isEmpty() const {
  if (m_store->sumOfColumn(1) == 0) {
    return true;
  }
  return false;
}

const I18n::Message *CalculationController::emptyMessage() {
  return &I18n::Common::NoValueToCompute;
}

Responder * CalculationController::defaultController() {
  return tabController();
}

int CalculationController::numberOfRows() {
  return k_totalNumberOfRows;
}

int CalculationController::numberOfColumns() {
  return 2;
}

void CalculationController::willDisplayCellAtLocation(HighlightCell * cell, int i, int j) {
  EvenOddCell * myCell = (EvenOddCell *)cell;
  myCell->setEven(j%2 == 0);
  myCell->setHighlighted(i == selectedColumn() && j == selectedRow());
  if (i == 0) {
    const I18n::Message *titles[k_totalNumberOfRows] = {&I18n::Common::TotalSize, &I18n::Common::Minimum, &I18n::Common::Maximum, &I18n::Common::Range, &I18n::Common::Mean, &I18n::Common::StandardDeviation, &I18n::Common::Deviation, &I18n::Common::FirstQuartile, &I18n::Common::ThirdQuartile, &I18n::Common::Median, &I18n::Common::InterquartileRange, &I18n::Common::Sum, &I18n::Common::SquareSum};
    EvenOddMessageTextCell * myCell = (EvenOddMessageTextCell *)cell;
    myCell->setMessage(titles[j]);
  } else {
    CalculPointer calculationMethods[k_totalNumberOfRows] = {&Store::sumOfOccurrences, &Store::minValue,
      &Store::maxValue, &Store::range, &Store::mean, &Store::standardDeviation, &Store::variance, &Store::firstQuartile,
      &Store::thirdQuartile, &Store::median, &Store::quartileRange, &Store::sum, &Store::squaredValueSum};
    double calculation = (m_store->*calculationMethods[j])();
    EvenOddBufferTextCell * myCell = (EvenOddBufferTextCell *)cell;
    char buffer[PrintFloat::bufferSizeForFloatsWithPrecision(Constant::LargeNumberOfSignificantDigits)];
    Complex<double>::convertFloatToText(calculation, buffer, PrintFloat::bufferSizeForFloatsWithPrecision(Constant::LargeNumberOfSignificantDigits), Constant::LargeNumberOfSignificantDigits);
    myCell->setText(buffer);
  }
}

KDCoordinate CalculationController::columnWidth(int i) {
  return k_cellWidth;
}

KDCoordinate CalculationController::rowHeight(int j) {
  return k_cellHeight;
}

HighlightCell * CalculationController::reusableCell(int index, int type) {
  assert(index < k_totalNumberOfRows);
  if (type == 0) {
    return m_titleCells[index];
  }
  return m_calculationCells[index];
}

int CalculationController::reusableCellCount(int type) {
  return k_maxNumberOfDisplayableRows;
}

int CalculationController::typeAtLocation(int i, int j) {
  return i;
}

Responder * CalculationController::tabController() const {
  return (parentResponder()->parentResponder()->parentResponder());
}

View * CalculationController::loadView() {
  for (int i = 0; i < k_maxNumberOfDisplayableRows; i++) {
    m_titleCells[i] = new EvenOddMessageTextCell(KDText::FontSize::Small);
    m_titleCells[i]->setAlignment(1.0f, 0.5f);
    m_calculationCells[i] = new EvenOddBufferTextCell(KDText::FontSize::Small);
    m_calculationCells[i]->setTextColor(Palette::GreyDark);
  }
  return TabTableController::loadView();
}


void CalculationController::unloadView(View * view) {
  for (int i = 0; i < k_maxNumberOfDisplayableRows; i++) {
    delete m_titleCells[i];
    m_titleCells[i] = nullptr;
    delete m_calculationCells[i];
    m_calculationCells[i] = nullptr;
  }
  TabTableController::unloadView(view);
}

}

