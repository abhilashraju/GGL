QT -= gui

CONFIG += c++1z
CONFIG -= app_bundle
QT += gui
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#        graphs.cpp
#        priorityqueue.cpp
#        trie.cpp
#        union_find.cpp
         main.cpp \
#         mst.cpp \
         dfs_bfs_gui.cpp \
         gs_apps.cpp \
         mst_gui.cpp \
         rasterwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += ../../src
HEADERS += \
    graph.hpp \
    graphalgorithms.hpp \
    priorityqueue.hpp \
    rasterwindow.h \
    trie.hpp \
    union_find.hpp