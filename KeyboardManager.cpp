#include "stdafx.h"
#include "KeyboardManager.h"

/******************************************/
/*           DEFINE DEFINITIONS           */
/******************************************/
#pragma region DEFINE

/* Stylesheet */
#define STYLE_TOOLTIP   "background-color:  rgba(56, 53, 51, 255);" \
                        "border:2px solid rgba(0, 0, 0, 100);"      \
                        "border-radius:%1px;"
#define STYLE_KEYBOARD  "background-color: #000000;"
#define STYLE_KEY       "QPushButton"                               \
                        "{"                                         \
                          "border: 1px transparent #000000;"        \
                          "border-radius: 5px;"                     \
                          "background-color: #383533;"              \
                          "color: #FFFFFF;"                         \
                        "}"                                         \
                        "QPushButton::hover"                        \
                        "{"                                         \
                          "background-color: #4D4C4A;"              \
                        "}"                                         \
                        "QPushButton::pressed"                      \
                        "{"                                         \
                          "background-color: #000000;"              \
                          "color: gray;"                            \
                        "}"                                         \
                        "QToolTip"                                  \
                        "{"                                         \
                          "background-color: #383533;"              \
                          "color: #FFFFFF;"                         \
                          "border-radius: 50px;"                    \
                          "border: 1px transparent #FFFFFF;"        \
                        "}"

/* Key Texts */
#define TEXT_NUMLOCK    ".123"
#define TEXT_LETTERS    "ABC"
#define TEXT_SPACE      "Space"

/* Qt use the same enum for both shift keys                       */
/* Qt::Key_Shift = 0x01000020                                     */
/* So we use Qt::Key_Shift - 1 for the right shift key            */
/* Just for internal use, Qt don't use this value in the enum     */
#define Key_LSHIFT Qt::Key_Shift
#define Key_RSHIFT Qt::Key_Shift - 1

/* Keys Icons */
#define ICON_KEYBOARD   ":/Keyboard/"

#define ICON_BACKSPACE  ICON_KEYBOARD "BACKSPACE.png"
#define ICON_RETURN     ICON_KEYBOARD "ENTER.png"
#define ICON_SHIFT_UP   ICON_KEYBOARD "SHIFT_UP.png"
#define ICON_SHIFT      ICON_KEYBOARD "SHIFT.png"
#define ICON_CAPS_LOCK  ICON_KEYBOARD "CAPS_LOCK.png"
#define ICON_CLOSE      ICON_KEYBOARD "CLOSE.png"
#define ICON_LANGUAGE   ICON_KEYBOARD "LANGUAGE.png"
#define ICON_LEFT       ICON_KEYBOARD "LEFT.png"
#define ICON_RIGHT      ICON_KEYBOARD "RIGHT.png"

#pragma endregion

/********************************************/
/*           CLASS IMPLEMENTATION           */
/********************************************/

KeyboardManager::KeyboardManager(QObject* parent)
{
  this->SetupLayouts();

  this->SetupUI();

  this->SetupEvent();

  this->resize(1200, 400);
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

#pragma region SETUP

void KeyboardManager::SetupUI()
{
  this->setStyleSheet(STYLE_KEYBOARD);

  this->m_MainLayout = new QVBoxLayout();
  this->setLayout(this->m_MainLayout);

  this->m_TooltipWidget = new QWidget(this);
  this->m_TooltipWidget->setAttribute(Qt::WA_TranslucentBackground);
  this->m_TooltipWidget->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);

  this->m_TooltipStyle = STYLE_TOOLTIP;

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
    { Qt::Key_NumLock, Qt::Key_Settings, Qt::Key_Space, Qt::Key_Left, Qt::Key_Right, Qt::Key_Close                                                          }
  };

  this->m_KeyLayouts[KEYBOARD_AZERTY] =
  {
    { Qt::Key_A,       Qt::Key_Z, Qt::Key_E, Qt::Key_R, Qt::Key_T, Qt::Key_Y, Qt::Key_U, Qt::Key_I, Qt::Key_O ,         Qt::Key_P,       Qt::Key_Backspace },
    { Qt::Key_Q,       Qt::Key_S, Qt::Key_D, Qt::Key_F, Qt::Key_G, Qt::Key_H, Qt::Key_J, Qt::Key_K, Qt::Key_L ,         Qt::Key_Return                     },
    { Key_LSHIFT,      Qt::Key_W, Qt::Key_X, Qt::Key_C, Qt::Key_V, Qt::Key_B, Qt::Key_N, Qt::Key_M, Qt::Key_Underscore, Qt::Key_Period,  Key_RSHIFT        },
    { Qt::Key_NumLock, Qt::Key_Settings, Qt::Key_Space, Qt::Key_Left, Qt::Key_Right, Qt::Key_Close                                                         }
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
    Qt::Key_Left, Qt::Key_Right
  };
}

#pragma endregion

#pragma region INIT

void KeyboardManager::InitButton(int key)
{
  KeyButton* button = new KeyButton();
  button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  button->setStyleSheet(STYLE_KEY);

  this->m_Buttons[key] = button;

  switch (key)
  {
  case Qt::Key_Backspace:       InitKeyBackspace();  break;
  case Qt::Key_Return:          InitKeyReturn();     break;
  case Key_RSHIFT:              InitKeyShift();      break;
  case Key_LSHIFT:           /* InitKeyShift(); */   break;
  case Qt::Key_Close:           InitKeyClose();      break;
  case Qt::Key_NumLock:         InitKeyNumLock();    break;
  case Qt::Key_Space:           InitKeySpace();      break;
  case Qt::Key_Settings:        InitKeySettings();   break;
  case Qt::Key_Left:            InitKeyLeft();       break;
  case Qt::Key_Right:           InitKeyRight();      break;
  default:
    button->setText(QKeySequence(key).toString().toLower());
    button->setToolTip(QKeySequence(key).toString().toLower());
    button->setToolTipDuration(1);
    break;
  }

  button->setMinimumSize(0, 0);

  QObject::connect(button, &QPushButton::pressed, this, &KeyboardManager::KeyPressed);
  QObject::connect(button, &QPushButton::clicked, this, &KeyboardManager::KeyClicked);
  QObject::connect(button, &QPushButton::released, this->m_TooltipWidget, &QWidget::hide);
}

void KeyboardManager::InitKeyBackspace()
{
  this->m_Buttons[Qt::Key_Backspace]->setIcon(QIcon(ICON_BACKSPACE));
}

void KeyboardManager::InitKeyReturn()
{
  this->m_Buttons[Qt::Key_Return]->setIcon(QIcon(ICON_RETURN));

}

void KeyboardManager::InitKeyShift()
{
  this->UpdateShiftIcons();
}

void KeyboardManager::InitKeyClose()
{
  this->m_Buttons[Qt::Key_Close]->setIcon(QIcon(ICON_CLOSE));
}

void KeyboardManager::InitKeyNumLock()
{
  this->m_Buttons[Qt::Key_NumLock]->setText(TEXT_NUMLOCK);
}

void KeyboardManager::InitKeySpace()
{
  this->m_Buttons[Qt::Key_Space]->setText(TEXT_SPACE);
}

void KeyboardManager::InitKeySettings()
{
  this->m_Buttons[Qt::Key_Settings]->setIcon(QIcon(ICON_LANGUAGE));
}

void KeyboardManager::InitKeyLeft()
{
  this->m_Buttons[Qt::Key_Left]->setIcon(QIcon(ICON_LEFT));
}

void KeyboardManager::InitKeyRight()
{
  this->m_Buttons[Qt::Key_Right]->setIcon(QIcon(ICON_RIGHT));
}

#pragma endregion

#pragma region EVENTS

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

  for (int i = 0; i < this->m_Layouts.size(); i++)
  {
    QHBoxLayout* layout = this->m_Layouts[i];
    if (layout == nullptr)
      continue;

    layout->setSpacing(spacingW);
  }

  QLayout* layout = this->m_Layouts[1];
  if (layout == nullptr)
    return;

  int minHeight = 999999;
  for (QPushButton* button : this->m_Buttons)
  {
    if (button == nullptr)
      continue;
    minHeight = qMin(minHeight, button->height());
  }

  QFont font = this->font();
  font.setPixelSize(minHeight * 0.5);

  for (int key : this->m_Buttons.keys())
  {
    if (this->m_MiscKeys.contains(key))
      continue;

    QPushButton* button = this->m_Buttons[key];
    if (button == nullptr)
      continue;

    button->setFont(font);
  }
  this->m_Buttons[Qt::Key_NumLock]->setFont(font);
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
      default:                                                                  continue;
      case Qt::Key_Backspace:                                                   break;
      case Qt::Key_Return:      button->setFixedWidth(size.width() * 0.133);    break;
      case Key_RSHIFT:          button->setFixedWidth(size.width() * 0.095);    break;
      case Key_LSHIFT:                                                          break;
      case Qt::Key_Close:       button->setFixedWidth(size.width() * 0.100);    break;
      case Qt::Key_NumLock:     button->setFixedWidth(size.width() * 0.100);    break;
      case Qt::Key_Space:       button->setFixedWidth(size.width() * 0.425);    break;
      case Qt::Key_Settings:                                                    break;
      case Qt::Key_Left:                                                        break;
      case Qt::Key_Right:                                                       break;
    }

    QIcon icon = button->icon();
    if (icon.isNull())
      continue;

    int size = button->height();
    QSize iconSize(size, size);
    button->setIconSize(iconSize);
  }

  layout = this->m_Layouts[1];
  if (layout == nullptr)
    return;

  if (this->m_CurrentLayout == KEYBOARD_NUMERIC)
    marginW = 0;

  layout->setContentsMargins(marginW, 0, 0, 0);
}

#pragma endregion

#pragma region UPDATE

void KeyboardManager::UpdateLayout(int layout)
{
  QList<QList<int>> keysByRow = this->m_KeyLayouts[layout];
  this->m_CurrentLayout = layout;

  if (layout != KEYBOARD_NUMERIC)
    this->m_CurrentLanguage = layout;

  this->ClearLayout();

  this->AddLayout(keysByRow.size());

  this->FillKeys(keysByRow);

  QResizeEvent* event = new QResizeEvent(this->size(), this->size());
  this->resizeEvent(event);
}

void KeyboardManager::UpdateTextKeys()
{
  for(int key : this->m_Buttons.keys())
  {
    if (this->m_MiscKeys.contains(key))
      continue;

    QPushButton* button = this->m_Buttons[key];
    if (button == nullptr)
      continue;

    QString text = button->text();
    switch (this->m_CurrentShift)
    {
      case SHIFT_DISABLE:
        text = QKeySequence(key).toString().toLower();
        break;

      case SHIFT_ENABLE:
        text = QKeySequence(key).toString().toUpper();
        break;

      case SHIFT_CAPS:
        text = QKeySequence(key).toString();
        break;
    }

    button->setText(text);
    button->setToolTip(text);
  }
}

void KeyboardManager::UpdateShiftIcons()
{
  switch (this->m_CurrentShift)
  {
    case SHIFT_DISABLE:
      this->m_Buttons[Key_LSHIFT]->setIcon(QIcon(ICON_SHIFT_UP));
      this->m_Buttons[Key_RSHIFT]->setIcon(QIcon(ICON_SHIFT_UP));
      break;

    case SHIFT_ENABLE:
      this->m_Buttons[Key_LSHIFT]->setIcon(QIcon(ICON_SHIFT));
      this->m_Buttons[Key_RSHIFT]->setIcon(QIcon(ICON_SHIFT));
      break;

    case SHIFT_CAPS:
      this->m_Buttons[Key_LSHIFT]->setIcon(QIcon(ICON_CAPS_LOCK));
      this->m_Buttons[Key_RSHIFT]->setIcon(QIcon(ICON_CAPS_LOCK));
      break;
  }
}

#pragma endregion

#pragma region LAYOUT

void KeyboardManager::SetKeyboardLayout(int layout)
{
  this->UpdateLayout(layout);
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
        this->InitButton(key);
        button = this->m_Buttons[key];
      }
      QString text = button->text();
      layout->addWidget(button);
    }
  }
}

void KeyboardManager::AddLayout(int rowCount)
{
  for (int row = 0; row < rowCount; row++)
  {
    QHBoxLayout* layout = this->m_Layouts[row];
    if (layout == nullptr)
    {
      this->m_Layouts[row] = GetRowLayout();
      layout = this->m_Layouts[row];
    }
    this->m_MainLayout->addLayout(layout);
  }
}

void KeyboardManager::ClearLayout()
{
  for (int row = 0; row < this->m_Layouts.size(); row++)
  {
    QHBoxLayout* layout = this->m_Layouts[row];

    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr)
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

QHBoxLayout* KeyboardManager::GetRowLayout()
{
  QHBoxLayout* layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  return layout;
}

#pragma endregion

#pragma region KEY EVENTS

void KeyboardManager::KeyPressed()
{
  KeyButton* button = qobject_cast<KeyButton*>(sender());
  if (button == nullptr)
    return;

  int key = this->m_Buttons.key(button);
  if (this->m_MiscKeys.contains(key))
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
  KeyButton* button = qobject_cast<KeyButton*>(sender());
  if (button == nullptr)
    return;

  int key = this->m_Buttons.key(button);
  if (this->m_MiscKeys.contains(key))
  {
    this->MiscKeyClicked(key);
    return;
  }

  this->m_KeySequence.insert(this->m_CursorPosition, button->text());
  this->m_CursorPosition++;

  qDebug() << this->m_KeySequence;

  if (this->m_CurrentShift != SHIFT_ENABLE)
    return;

  if(this->m_CurrentLayout == KEYBOARD_NUMERIC)
    return;

  this->m_CurrentShift = SHIFT_DISABLE;
  this->UpdateTextKeys();
  this->UpdateShiftIcons();
}

void KeyboardManager::MiscKeyClicked(int key)
{
  switch (key)
  {
    case Qt::Key_Backspace:   this->KeyBackspaceClicked();   break;
    case Qt::Key_Return:      this->KeyReturnClicked();      break;
    case Key_RSHIFT:          this->KeyShiftClicked();       break;
    case Qt::Key_Close:       this->KeyCloseClicked();       break;
    case Key_LSHIFT:          this->KeyShiftClicked();       break;
    case Qt::Key_NumLock:     this->KeyLayoutClicked();      break;
    case Qt::Key_Space:       this->KeySpaceClicked();       break;
    case Qt::Key_Settings:    this->KeySettingsClicked();    break;
    case Qt::Key_Left:        this->KeyLeftClicked();        break;
    case Qt::Key_Right:       this->KeyRightClicked();       break;
//    case Qt::Key_Guide:   this->GuideClicked();       break;
//    case Qt::Key_Info:    this->InfoClicked();        break;
  }
}

void KeyboardManager::KeyBackspaceClicked()
{
  this->m_KeySequence.chop(1);

  if (this->m_CursorPosition == 0)
    return;

  this->m_CursorPosition--;
}

void KeyboardManager::KeyReturnClicked()
{
  emit this->SendKeySequence(this->m_KeySequence);
  qDebug() << "emitted: " << this->m_KeySequence;
  this->m_KeySequence.clear();
  this->m_CursorPosition = 0;
}

void KeyboardManager::KeyShiftClicked()
{
  switch (this->m_CurrentShift)
  {
    case SHIFT_DISABLE:   this->m_CurrentShift = SHIFT_ENABLE;    break;
    case SHIFT_ENABLE:    this->m_CurrentShift = SHIFT_CAPS;      break;
    case SHIFT_CAPS:      this->m_CurrentShift = SHIFT_DISABLE;   break;
  }

  this->UpdateTextKeys();
  this->UpdateShiftIcons();
}

void KeyboardManager::KeyCloseClicked()
{
  this->hide();
}

void KeyboardManager::KeyLayoutClicked()
{
  switch (this->m_CurrentLayout)
  {
    case KEYBOARD_QWERTY:
      this->UpdateLayout(KEYBOARD_NUMERIC);
      this->m_Buttons[Qt::Key_NumLock]->setText(TEXT_LETTERS);
      break;

    case KEYBOARD_AZERTY:
      this->UpdateLayout(KEYBOARD_NUMERIC);
      this->m_Buttons[Qt::Key_NumLock]->setText(TEXT_LETTERS);
      break;

    case KEYBOARD_NUMERIC:
      this->UpdateLayout(this->m_CurrentLanguage);
      this->m_Buttons[Qt::Key_NumLock]->setText(TEXT_NUMLOCK);
      break;
  }
}

void KeyboardManager::KeySpaceClicked()
{
  this->m_KeySequence.insert(this->m_CursorPosition, " ");
  this->m_CursorPosition++;
}

void KeyboardManager::KeySettingsClicked()
{
  switch (this->m_CurrentLanguage)
  {
    case KEYBOARD_QWERTY: this->UpdateLayout(KEYBOARD_AZERTY); break;
    case KEYBOARD_AZERTY: this->UpdateLayout(KEYBOARD_QWERTY); break;
  }

  this->m_CurrentShift = SHIFT_DISABLE;
  this->UpdateTextKeys();
  this->UpdateShiftIcons();
}

void KeyboardManager::KeyLeftClicked()
{
  if (this->m_CursorPosition == 0)
    return;

  this->m_CursorPosition--;
}

void KeyboardManager::KeyRightClicked()
{
  if(this->m_CursorPosition == this->m_KeySequence.size())
    return;

  this->m_CursorPosition++;
}

#pragma endregion

#pragma region KEY BUTTON

QSize KeyButton::minimumSizeHint() const
{
  return QSize(1, 1);
}

#pragma endregion