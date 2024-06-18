#include "stdafx.h"
#include "KeyboardManager.h"

// Qt use the same enum for both shift keys
// Qt::Key_Shift = 0x01000020
// So we use Qt::Key_Shift - 1 for the right shift key
// Just for internal use, Qt don't use this value in the enum

#define Key_LSHIFT Qt::Key_Shift
#define Key_RSHIFT Qt::Key_Shift - 1

KeyboardManager::KeyboardManager(QObject* parent)
{
  this->SetupLayouts();

  this->SetupUI();

  this->SetupEvent();
}

KeyboardManager::~KeyboardManager()
{
}

KeyboardManager* KeyboardManager::Instance()
{
  // Create a static instance of ThemesManager (singleton pattern)
  static KeyboardManager* Instance = new KeyboardManager();
  return Instance;
}

void KeyboardManager::SetKeyboardLayout(int layout)
{
  this->UpdateLayout(layout);
}

void KeyboardManager::SetupUI()
{
  this->setStyleSheet("background-color: #000000;");

  this->m_MainLayout = new QVBoxLayout();
  this->setLayout(this->m_MainLayout);

  this->m_TooltipWidget = new QWidget(this);
  this->m_TooltipWidget->setAttribute(Qt::WA_TranslucentBackground);
  this->m_TooltipWidget->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

  this->m_TooltipStyle = "background-color:  rgba(56, 53, 51, 255); border:2px solid rgba(0, 0, 0, 100); border-radius:%1px;";

  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  this->m_TooltipLabel = new QLabel();
  this->m_TooltipLabel->setAlignment(Qt::AlignCenter);
  this->m_TooltipLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  
  layout->addWidget(this->m_TooltipLabel);
  this->m_TooltipWidget->setLayout(layout);

  this->UpdateLayout(KEYBOARD_QWERTY);
}

void KeyboardManager::SetupEvent()
{
}

void KeyboardManager::SetupLayouts()
{
  this->m_KeyLayouts[KEYBOARD_QWERTY] =
  {
    { Qt::Key_Q,       Qt::Key_W, Qt::Key_E, Qt::Key_R, Qt::Key_T, Qt::Key_Y, Qt::Key_U, Qt::Key_I, Qt::Key_O,          Qt::Key_P,       Qt::Key_Backspace },
    { Qt::Key_A,       Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_G, Qt::Key_H, Qt::Key_J, Qt::Key_K, Qt::Key_L,          Qt::Key_Return                     },
    { Key_LSHIFT,      Qt::Key_Z, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N, Qt::Key_M, Qt::Key_Underscore, Qt::Key_Period,  Key_RSHIFT        },
    { Qt::Key_NumLock, Qt::Key_Settings, Qt::Key_Space, Qt::Key_Guide, Qt::Key_Info, Qt::Key_Close                                                                }
  };

  this->m_KeyLayouts[KEYBOARD_AZERTY] =
  {
    { Qt::Key_A,       Qt::Key_Z, Qt::Key_E, Qt::Key_R, Qt::Key_T, Qt::Key_Y, Qt::Key_U, Qt::Key_I, Qt::Key_O ,         Qt::Key_P,       Qt::Key_Backspace },
    { Qt::Key_Q,       Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_G, Qt::Key_H, Qt::Key_J, Qt::Key_K, Qt::Key_L ,         Qt::Key_Return                     },
    { Key_LSHIFT,      Qt::Key_W, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N, Qt::Key_M, Qt::Key_Underscore, Qt::Key_Period,  Key_RSHIFT        },
    { Qt::Key_NumLock, Qt::Key_Settings, Qt::Key_Space, Qt::Key_Guide, Qt::Key_Info, Qt::Key_Close                                                                }
  };
  
  this->m_KeyLayouts[KEYBOARD_NUMERIC] =
  {
    { Qt::Key_7,        Qt::Key_8, Qt::Key_9,       Qt::Key_Backspace   },
    { Qt::Key_4,        Qt::Key_5, Qt::Key_6,       Qt::Key_Underscore  },
    { Qt::Key_1,        Qt::Key_2, Qt::Key_3,       Qt::Key_Close       },
    { Qt::Key_NumLock,  Qt::Key_0, Qt::Key_Period,  Qt::Key_Return      }
  };

  this->m_MiscKeys =
  {
    Qt::Key_Backspace, Qt::Key_Return, Key_RSHIFT, Qt::Key_Close,
    Key_LSHIFT, Qt::Key_NumLock, Qt::Key_Settings, Qt::Key_Space,
    Qt::Key_Guide, Qt::Key_Info
  };
}

void KeyboardManager::UpdateLayout(int layout)
{
  QList<QList<int>> keysByRow = this->m_KeyLayouts[layout];
  this->m_CurrentLayout = layout;

  this->ClearLayout();

  this->AddLayout(keysByRow.size());

  this->FillKeys(keysByRow);

  QResizeEvent* event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
}

QHBoxLayout* KeyboardManager::GetRowLayout()
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  return layout;
}

QPushButton* KeyboardManager::GetButton(int key)
{
  QPushButton* button = new QPushButton();
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  button->setStyleSheet
  (
    "QPushButton"
    "{"
      "border: 1px transparent #000000;"
      "border-radius: 5px;"
      "background-color: #383533;"
      "color: #FFFFFF;"
    "}"
    "QPushButton::hover"
    "{"
      "background-color: #4D4C4A;"
    "}"
    "QPushButton::pressed"
    "{"
      "background-color: #000000;"
      "color: gray;"
    "}"
    "QToolTip"
    "{"
      "background-color: #383533;"
      "color: #FFFFFF;"
      "border-radius: 50px;"
      "border: 1px transparent #FFFFFF;"
    "}"
  );

  switch (key)
  {
    case Key_RSHIFT:
    case Key_LSHIFT:
      button->setText("Shift");
      break;

    default:
      button->setText(QKeySequence(key).toString());
      button->setToolTip(QKeySequence(key).toString());
      button->setToolTipDuration(1);
      break;
  }

  QObject::connect(button, &QPushButton::pressed, this, &KeyboardManager::KeyPressed);
  QObject::connect(button, &QPushButton::clicked, this, &KeyboardManager::KeyClicked);
  QObject::connect(button, &QPushButton::released, this->m_TooltipWidget, &QWidget::hide);
  return button;
}

void KeyboardManager::ClearLayout()
{
  for (int row = 0; row < this->m_Layouts.size(); row++)
  {
    QHBoxLayout* layout = this->m_Layouts[row];

    QLayoutItem* item;
    while((item = layout->takeAt(0)) != nullptr)
    {
      // Clear only layout items not widgets
      QWidget* widget = item->widget();
      widget->setParent(nullptr);
      if (widget == nullptr)
        delete item;
    }

    layout->invalidate();

    this->m_MainLayout->removeItem(layout);
  }
}

void KeyboardManager::AddLayout(int rowCount)
{
  for (int row = 0; row < rowCount; row++)
  {
    QHBoxLayout* layout = this->m_Layouts[row];
    if(layout == nullptr)
    {
      this->m_Layouts[row] = GetRowLayout();
      layout = this->m_Layouts[row];
    }
    this->m_MainLayout->addLayout(layout);
  }
}

void KeyboardManager::FillKeys(QList<QList<int>>& keysByRow)
{
  for (int row = 0; row < keysByRow.size(); row++)
  {
    QHBoxLayout* layout = this->m_Layouts[row];
    QList<int> keys = keysByRow[row];

    for (int column = 0; column < keys.size(); column++)
    {
      int key = keys[column];
      QPushButton* button = this->m_Buttons[key];
      if (button == nullptr)
      {
        this->m_Buttons[key] = this->GetButton(key);
        button = this->m_Buttons[key];
      }
      QString text = button->text();
      layout->addWidget(button);
    }
  }
}

void KeyboardManager::resizeEvent(QResizeEvent* event)
{
  QSize size = event->size();

  int marginW = 0.05 * size.width();
  int spacingW = 0.01 * size.width();
  int spacingH = spacingW;
  int marginTop = spacingW;
  int marginBottom = 0.07 * size.height();

  this->m_MainLayout->setContentsMargins(marginW, marginTop, marginW, marginBottom);
  this->m_MainLayout->setSpacing(spacingH);

  //int columnCount = 0;
  for(int i = 0; i < this->m_Layouts.size(); i++)
  {
    QHBoxLayout* layout = this->m_Layouts[i];
    if (layout == nullptr)
      continue;

    layout->setSpacing(spacingW);
    //columnCount = qMax(columnCount, layout->count());
  }

  QLayout* layout = this->m_Layouts[1];
  if (layout == nullptr)
    return;

  QFont font = this->font();
  font.setPixelSize(size.width() * 0.025);

  for(int key : this->m_Buttons.keys())
  {
    if (this->m_MiscKeys.contains(key))
      continue;

    QPushButton* button = this->m_Buttons[key];
    if (button == nullptr)
      continue;

    button->setFont(font);
  }
  this->m_Buttons[Qt::Key_Space]->setFont(font);

  for (int key : this->m_MiscKeys)
  {
    QPushButton* button = this->m_Buttons[key];
    if (button == nullptr)
      continue;

    if (this->m_CurrentLayout == KEYBOARD_NUMERIC)
    {
      button->setMinimumSize(0, 0);
      button->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
      continue;
    }

    switch (key)
    {
      case Qt::Key_Return:  button->setFixedWidth(size.width()    * 0.133);    break;
      case Key_RSHIFT:      button->setFixedWidth(size.width()    * 0.095);    break;
      case Qt::Key_Close:   button->setFixedWidth(size.width()    * 0.100);    break;
      case Qt::Key_NumLock: button->setFixedWidth(size.width()    * 0.100);    break;
      case Qt::Key_Space:   button->setFixedWidth(size.width()    * 0.425);    break;
    }
  }

  layout = this->m_Layouts[1];
  if (layout == nullptr)
    return;

  if (this->m_CurrentLayout == KEYBOARD_NUMERIC)
    marginW = 0;

  layout->setContentsMargins(marginW, 0, 0, 0);
}

void KeyboardManager::KeyPressed()
{
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  if (button == nullptr)
    return;

  int key = this->m_Buttons.key(button);
  if(this->m_MiscKeys.contains(key))
    return;

  int size = qMin(button->width(), button->height()) * 2;
  int radius = size / 2;

  int x = (button->width() / 2) - radius;

  QPoint pos = button->mapToGlobal(QPoint(x, -size));
  if (pos.y() < 0)
    pos = button->mapToGlobal(QPoint(x, button->height()));

  QFont font = button->font();
  font.setPixelSize(font.pixelSize() * 1.5);

  this->m_TooltipLabel->setFont(font);
  this->m_TooltipLabel->setText(button->text());
  this->m_TooltipWidget->setStyleSheet(this->m_TooltipStyle.arg(radius));

  this->m_TooltipWidget->setFixedSize(size, size);
  this->m_TooltipWidget->move(pos);
  this->m_TooltipWidget->show();
}

void KeyboardManager::KeyClicked()
{
  QPushButton* button = qobject_cast<QPushButton*>(sender());
  if (button == nullptr)
    return;

  int key = this->m_Buttons.key(button);
  qDebug() << "KeyClicked: " << QKeySequence(key).toString();

  if (key == Qt::Key_NumLock && this->m_CurrentLayout != KEYBOARD_NUMERIC)
    this->UpdateLayout(KEYBOARD_NUMERIC);
  else if(key == Qt::Key_NumLock)
    this->UpdateLayout(KEYBOARD_QWERTY);
}