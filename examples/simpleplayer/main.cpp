/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2015 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include <QApplication>
#include "playerwindow.h"
#include <QtAVWidgets>
#include <QFile>
#include <QTextStream>

// #ifdef _DEBUG
//     #define new new (_NORMAL_BLOCK, __FILE__, __LINE__) // 定义new宏来替换new关键字


void outLogMessageToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString message;
    QString app_run_addr;
    message = qFormatLogMessage(type, context, msg);
    message.append("\r\n");

    //获取程序当前运行目录
    QString current_PathName = QCoreApplication::applicationDirPath();
    if(QFile::exists(current_PathName)==false)
    {
        app_run_addr = "debug.log";
    }else
    {
        app_run_addr = current_PathName + "/"+"debug.log";
    }
    QFile file(app_run_addr);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(message.toLocal8Bit());
    }
    file.close();
}


int main(int argc, char *argv[])
{

// #ifdef _MSC_VER
//     // _CrtSetBreakAlloc(590);
//     _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
//     _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
//     _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
//     #ifdef MEMORY_STDOUT // 是否将内存泄露信息，存储到文件中
//         _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
//         _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
//         _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
//     #else
//         HANDLE hLogFile;
//         hLogFile = CreateFile(L"./memory.txt", GENERIC_WRITE,
//                             FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
//                             FILE_ATTRIBUTE_NORMAL, NULL);
//         _CrtSetReportFile(_CRT_WARN, hLogFile);
//         _CrtSetReportFile(_CRT_ERROR, hLogFile);
//         _CrtSetReportFile(_CRT_ASSERT, hLogFile);
//     #endif // MEMORY_STDOUT
//     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

// #endif // _MSC_VER

    QtAV::Widgets::registerRenderers();
    QApplication a(argc, argv);
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{message}");
    //初始化qdebug的输出重定向到文件
    qInstallMessageHandler(outLogMessageToFile);
    PlayerWindow player;
    player.show();
    player.resize(800, 600);
    return a.exec();
}

// #endif
