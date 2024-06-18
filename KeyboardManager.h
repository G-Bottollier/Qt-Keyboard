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

/* class definition */
class InputKey;

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

  private:
    int                          m_CurrentLayout = -1;
    QVBoxLayout*                 m_MainLayout    = nullptr;

    QString                      m_TooltipStyle;
    QWidget*                     m_TooltipWidget = nullptr;
    QLabel*                      m_TooltipLabel  = nullptr;

    QMap<int, QHBoxLayout*>      m_Layouts;
    QMap<int, QList<QList<int>>> m_KeyLayouts;
    QMap<int, QPushButton*>      m_Buttons;
    QList<int>                   m_MiscKeys;

    void SetupUI();
    void SetupEvent();
    void SetupLayouts();

    void UpdateLayout(int layout);
    void ClearLayout();
    void AddLayout(int rowCount);
    void FillKeys(QList<QList<int>>& keysByRow);

    QHBoxLayout* GetRowLayout();
    QPushButton* GetButton(int key);

  protected:
    void resizeEvent(QResizeEvent* event) override;

  private:
    // Constructor
    explicit KeyboardManager(QObject* parent = nullptr);

  public:
    // Destructor
    ~KeyboardManager();

    static KeyboardManager* Instance();

    void SetKeyboardLayout(int layout);

  public slots:
    void KeyPressed();
    void KeyClicked();
};

#endif // KEYBOARDMANAGER_H
