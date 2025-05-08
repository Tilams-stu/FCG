/****************************************************************************
** Meta object code from reading C++ file 'controlpanel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../FCGClient/view/controlpanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'controlpanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSControlPanelENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSControlPanelENDCLASS = QtMocHelpers::stringData(
    "ControlPanel",
    "readyClicked",
    "",
    "rollDiceClicked",
    "planeSelected",
    "planeId",
    "flyOverChoice",
    "accepted",
    "handleReady",
    "handleRollDice",
    "handlePlaneButton",
    "id",
    "handleFlyOver",
    "yes",
    "GamePhase",
    "ROLL_AND_CHOOSE_PLANE",
    "CHOOSE_FLY_OVER",
    "WAITING"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSControlPanelENDCLASS_t {
    uint offsetsAndSizes[36];
    char stringdata0[13];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[14];
    char stringdata5[8];
    char stringdata6[14];
    char stringdata7[9];
    char stringdata8[12];
    char stringdata9[15];
    char stringdata10[18];
    char stringdata11[3];
    char stringdata12[14];
    char stringdata13[4];
    char stringdata14[10];
    char stringdata15[22];
    char stringdata16[16];
    char stringdata17[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSControlPanelENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSControlPanelENDCLASS_t qt_meta_stringdata_CLASSControlPanelENDCLASS = {
    {
        QT_MOC_LITERAL(0, 12),  // "ControlPanel"
        QT_MOC_LITERAL(13, 12),  // "readyClicked"
        QT_MOC_LITERAL(26, 0),  // ""
        QT_MOC_LITERAL(27, 15),  // "rollDiceClicked"
        QT_MOC_LITERAL(43, 13),  // "planeSelected"
        QT_MOC_LITERAL(57, 7),  // "planeId"
        QT_MOC_LITERAL(65, 13),  // "flyOverChoice"
        QT_MOC_LITERAL(79, 8),  // "accepted"
        QT_MOC_LITERAL(88, 11),  // "handleReady"
        QT_MOC_LITERAL(100, 14),  // "handleRollDice"
        QT_MOC_LITERAL(115, 17),  // "handlePlaneButton"
        QT_MOC_LITERAL(133, 2),  // "id"
        QT_MOC_LITERAL(136, 13),  // "handleFlyOver"
        QT_MOC_LITERAL(150, 3),  // "yes"
        QT_MOC_LITERAL(154, 9),  // "GamePhase"
        QT_MOC_LITERAL(164, 21),  // "ROLL_AND_CHOOSE_PLANE"
        QT_MOC_LITERAL(186, 15),  // "CHOOSE_FLY_OVER"
        QT_MOC_LITERAL(202, 7)   // "WAITING"
    },
    "ControlPanel",
    "readyClicked",
    "",
    "rollDiceClicked",
    "planeSelected",
    "planeId",
    "flyOverChoice",
    "accepted",
    "handleReady",
    "handleRollDice",
    "handlePlaneButton",
    "id",
    "handleFlyOver",
    "yes",
    "GamePhase",
    "ROLL_AND_CHOOSE_PLANE",
    "CHOOSE_FLY_OVER",
    "WAITING"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSControlPanelENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       1,   78, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   62,    2, 0x06,    1 /* Public */,
       3,    0,   63,    2, 0x06,    2 /* Public */,
       4,    1,   64,    2, 0x06,    3 /* Public */,
       6,    1,   67,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   70,    2, 0x08,    7 /* Private */,
       9,    0,   71,    2, 0x08,    8 /* Private */,
      10,    1,   72,    2, 0x08,    9 /* Private */,
      12,    1,   75,    2, 0x08,   11 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void, QMetaType::Bool,   13,

 // enums: name, alias, flags, count, data
      14,   14, 0x0,    3,   83,

 // enum data: key, value
      15, uint(ControlPanel::ROLL_AND_CHOOSE_PLANE),
      16, uint(ControlPanel::CHOOSE_FLY_OVER),
      17, uint(ControlPanel::WAITING),

       0        // eod
};

Q_CONSTINIT const QMetaObject ControlPanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSControlPanelENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSControlPanelENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSControlPanelENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ControlPanel, std::true_type>,
        // method 'readyClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rollDiceClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'planeSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'flyOverChoice'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handleReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleRollDice'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handlePlaneButton'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'handleFlyOver'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>
    >,
    nullptr
} };

void ControlPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ControlPanel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->readyClicked(); break;
        case 1: _t->rollDiceClicked(); break;
        case 2: _t->planeSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->flyOverChoice((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->handleReady(); break;
        case 5: _t->handleRollDice(); break;
        case 6: _t->handlePlaneButton((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->handleFlyOver((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::readyClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)();
            if (_t _q_method = &ControlPanel::rollDiceClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)(int );
            if (_t _q_method = &ControlPanel::planeSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ControlPanel::*)(bool );
            if (_t _q_method = &ControlPanel::flyOverChoice; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *ControlPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ControlPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSControlPanelENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ControlPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ControlPanel::readyClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ControlPanel::rollDiceClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ControlPanel::planeSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ControlPanel::flyOverChoice(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
