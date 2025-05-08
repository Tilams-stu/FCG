/****************************************************************************
** Meta object code from reading C++ file 'gamecontroller.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../FCGClient/controller/gamecontroller.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamecontroller.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSGameControllerENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSGameControllerENDCLASS = QtMocHelpers::stringData(
    "GameController",
    "gameStateUpdated",
    "",
    "QMap<int,QList<int>>",
    "tileStates",
    "serverMessageReceived",
    "message",
    "connectionStatusChanged",
    "connected",
    "updateGamePhase",
    "ControlPanel::GamePhase",
    "phase",
    "sendReady",
    "sendPlaneOperation",
    "dice",
    "planeId",
    "sendFlyOverChoice",
    "isYes",
    "handleConnected",
    "handleReadyRead",
    "handleError",
    "QAbstractSocket::SocketError",
    "error",
    "handleDisconnected"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSGameControllerENDCLASS_t {
    uint offsetsAndSizes[48];
    char stringdata0[15];
    char stringdata1[17];
    char stringdata2[1];
    char stringdata3[21];
    char stringdata4[11];
    char stringdata5[22];
    char stringdata6[8];
    char stringdata7[24];
    char stringdata8[10];
    char stringdata9[16];
    char stringdata10[24];
    char stringdata11[6];
    char stringdata12[10];
    char stringdata13[19];
    char stringdata14[5];
    char stringdata15[8];
    char stringdata16[18];
    char stringdata17[6];
    char stringdata18[16];
    char stringdata19[16];
    char stringdata20[12];
    char stringdata21[29];
    char stringdata22[6];
    char stringdata23[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSGameControllerENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSGameControllerENDCLASS_t qt_meta_stringdata_CLASSGameControllerENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "GameController"
        QT_MOC_LITERAL(15, 16),  // "gameStateUpdated"
        QT_MOC_LITERAL(32, 0),  // ""
        QT_MOC_LITERAL(33, 20),  // "QMap<int,QList<int>>"
        QT_MOC_LITERAL(54, 10),  // "tileStates"
        QT_MOC_LITERAL(65, 21),  // "serverMessageReceived"
        QT_MOC_LITERAL(87, 7),  // "message"
        QT_MOC_LITERAL(95, 23),  // "connectionStatusChanged"
        QT_MOC_LITERAL(119, 9),  // "connected"
        QT_MOC_LITERAL(129, 15),  // "updateGamePhase"
        QT_MOC_LITERAL(145, 23),  // "ControlPanel::GamePhase"
        QT_MOC_LITERAL(169, 5),  // "phase"
        QT_MOC_LITERAL(175, 9),  // "sendReady"
        QT_MOC_LITERAL(185, 18),  // "sendPlaneOperation"
        QT_MOC_LITERAL(204, 4),  // "dice"
        QT_MOC_LITERAL(209, 7),  // "planeId"
        QT_MOC_LITERAL(217, 17),  // "sendFlyOverChoice"
        QT_MOC_LITERAL(235, 5),  // "isYes"
        QT_MOC_LITERAL(241, 15),  // "handleConnected"
        QT_MOC_LITERAL(257, 15),  // "handleReadyRead"
        QT_MOC_LITERAL(273, 11),  // "handleError"
        QT_MOC_LITERAL(285, 28),  // "QAbstractSocket::SocketError"
        QT_MOC_LITERAL(314, 5),  // "error"
        QT_MOC_LITERAL(320, 18)   // "handleDisconnected"
    },
    "GameController",
    "gameStateUpdated",
    "",
    "QMap<int,QList<int>>",
    "tileStates",
    "serverMessageReceived",
    "message",
    "connectionStatusChanged",
    "connected",
    "updateGamePhase",
    "ControlPanel::GamePhase",
    "phase",
    "sendReady",
    "sendPlaneOperation",
    "dice",
    "planeId",
    "sendFlyOverChoice",
    "isYes",
    "handleConnected",
    "handleReadyRead",
    "handleError",
    "QAbstractSocket::SocketError",
    "error",
    "handleDisconnected"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSGameControllerENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   80,    2, 0x06,    1 /* Public */,
       5,    1,   83,    2, 0x06,    3 /* Public */,
       7,    1,   86,    2, 0x06,    5 /* Public */,
       9,    2,   89,    2, 0x06,    7 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,   94,    2, 0x0a,   10 /* Public */,
      13,    2,   95,    2, 0x0a,   11 /* Public */,
      16,    1,  100,    2, 0x0a,   14 /* Public */,
      18,    0,  103,    2, 0x08,   16 /* Private */,
      19,    0,  104,    2, 0x08,   17 /* Private */,
      20,    1,  105,    2, 0x08,   18 /* Private */,
      23,    0,  108,    2, 0x08,   20 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::QString,   11,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject GameController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_CLASSGameControllerENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSGameControllerENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSGameControllerENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<GameController, std::true_type>,
        // method 'gameStateUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QMap<int,QList<int>> &, std::false_type>,
        // method 'serverMessageReceived'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'connectionStatusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateGamePhase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<ControlPanel::GamePhase, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'sendReady'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendPlaneOperation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'sendFlyOverChoice'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'handleConnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'handleError'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>,
        // method 'handleDisconnected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void GameController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GameController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->gameStateUpdated((*reinterpret_cast< std::add_pointer_t<QMap<int,QList<int>>>>(_a[1]))); break;
        case 1: _t->serverMessageReceived((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->connectionStatusChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->updateGamePhase((*reinterpret_cast< std::add_pointer_t<ControlPanel::GamePhase>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->sendReady(); break;
        case 5: _t->sendPlaneOperation((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->sendFlyOverChoice((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 7: _t->handleConnected(); break;
        case 8: _t->handleReadyRead(); break;
        case 9: _t->handleError((*reinterpret_cast< std::add_pointer_t<QAbstractSocket::SocketError>>(_a[1]))); break;
        case 10: _t->handleDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 9:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GameController::*)(const QMap<int,QList<int>> & );
            if (_t _q_method = &GameController::gameStateUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GameController::*)(const QString & );
            if (_t _q_method = &GameController::serverMessageReceived; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GameController::*)(bool );
            if (_t _q_method = &GameController::connectionStatusChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GameController::*)(ControlPanel::GamePhase , const QString & );
            if (_t _q_method = &GameController::updateGamePhase; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *GameController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSGameControllerENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GameController::gameStateUpdated(const QMap<int,QList<int>> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GameController::serverMessageReceived(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GameController::connectionStatusChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GameController::updateGamePhase(ControlPanel::GamePhase _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
