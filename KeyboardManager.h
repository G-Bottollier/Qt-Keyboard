#ifndef KEYBOARDMANAGER_H
#define KEYBOARDMANAGER_H

#include <QtWidgets>

/**
  * @ingroup CommandsManager
  *
  * @def qCommands
  * This is a convenience macro to access to the unique KeyboardManager instance.
  *
  * @sa
  * @link KeyboardManager::Instance Instance()@endlink
  * */
#define qKeyboard (static_cast<KeyboardManager*>(KeyboardManager::Instance()))

#define KEYBOARD_QWERTY   0
#define KEYBOARD_AZERTY   1
#define KEYBOARD_NUMERIC  2

#define SHIFT_DISABLE     0
#define SHIFT_ENABLE      1
#define SHIFT_CAPS        2

/* class definition */
class KeyButton;

/**
  * @defgroup KeyboardManager KeyboardManager
  * @nosubgrouping
  * @ingroup Manager
  *
  * It provides a virtual keyboard with different layouts.
  * The user can switch between layouts by clicking on the corresponding button.
  *
  * Current layouts supported are:
  * - QWERTY.
  * - AZERTY.
  * - Numeric.
  *
  * Key capabilities include:
  * - Display a tooltip over the pressed key.
  * - Can switch between layouts at runtime.
  * - Automatically resize the keyboard.
  * - Provides a virtual keyboard.
  * - Provides different layout.
  *
  * @note The class is still work in progress.
  *
  * The KeyboardManager object can be accessed through a unique global instance with @ref qKeyboard.
  *
  * @sa
  * @link qKeyboard qKeyboard@endlink
  * */
class KeyboardManager : public QWidget
{
  Q_OBJECT

  #pragma region MEMBERS

  private:
    int                          m_CurrentLayout   = 0;
    int                          m_CurrentShift    = 0;
    int                          m_CurrentLanguage = 0;
    int                          m_CursorPosition  = 0;
    QVBoxLayout*                 m_MainLayout      = nullptr;

    QString                      m_KeySequence;

    QString                      m_TooltipStyle;
    QWidget*                     m_TooltipWidget   = nullptr;
    QLabel*                      m_TooltipLabel    = nullptr;

    QMap<int, QHBoxLayout*>      m_Layouts;
    QMap<int, QList<QList<int>>> m_KeyLayouts;
    QMap<int, KeyButton*>        m_Buttons;
    QList<int>                   m_MiscKeys;

  #pragma endregion

  #pragma region SETUP

  private:
    void SetupUI();
    void SetupEvent();
    void SetupLayouts();

  #pragma endregion

  #pragma region INIT

  private:
    void InitButton(int key);
    void InitKeyBackspace();
    void InitKeyReturn();
    void InitKeyShift();
    void InitKeyClose();
    void InitKeyNumLock();
    void InitKeySpace();
    void InitKeySettings();
    void InitKeyLeft();
    void InitKeyRight();

  #pragma endregion

  #pragma region UPDATE

  private:
    void UpdateLayout(int layout);
    void UpdateTextKeys();
    void UpdateShiftIcons();

  #pragma endregion

  #pragma region LAYOUT

  private:
    void FillKeys(QList<QList<int>>& keysByRow);
    void AddLayout(int rowCount);
    void ClearLayout();
    QHBoxLayout* GetRowLayout();

  public:
    void SetKeyboardLayout(int layout);

  #pragma endregion

  #pragma region EVENTS

  protected:
    void resizeEvent(QResizeEvent* event) override;

  #pragma endregion

  private:
    explicit KeyboardManager(QObject* parent = nullptr);

  public:
    ~KeyboardManager();

    static KeyboardManager* Instance();

  #pragma region KEY EVENTS

  public slots:
    void KeyPressed();
    void KeyClicked();
    void MiscKeyClicked(int key);

    /* Keys clicked */
    void KeyBackspaceClicked();
    void KeyReturnClicked();
    void KeyShiftClicked();
    void KeyCloseClicked();
    void KeyLayoutClicked();
    void KeySpaceClicked();
    void KeySettingsClicked();
    void KeyLeftClicked();
    void KeyRightClicked();

  #pragma endregion

  signals:
    void SendKeySequence(QString str);
};

#pragma region KEY BUTTON

class KeyButton : public QPushButton
{
  Q_OBJECT

  public:
    QSize	minimumSizeHint() const override;
};

#pragma endregion

#endif // KEYBOARDMANAGER_H
