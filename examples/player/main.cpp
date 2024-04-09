/******************************************************************************
    QtAV Player Demo:  this file is part of QtAV examples
    Copyright (C) 2012-2022 Wang Bin <wbsecg1@gmail.com>

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
#include <QtDebug>
#include <QtCore/QDir>
#include <QMessageBox>

#include <QtAV/AVPlayer.h>
#include <QtAV/VideoOutput.h>
#include <QtAVWidgets>
#include "config/PropertyEditor.h"
#include "MainWindow.h"
#include "../common/common.h"

using namespace QtAV;
static const struct {
    const char* name;
    VideoRendererId id;
} vid_map[] = {
{ "opengl", VideoRendererId_OpenGLWidget },
{ "gl", VideoRendererId_GLWidget2 },
{ "d2d", VideoRendererId_Direct2D },
{ "gdi", VideoRendererId_GDI },
{ "xv", VideoRendererId_XV },
{ "x11", VideoRendererId_X11 },
{ "qt", VideoRendererId_Widget },
{ 0, 0 }
};

VideoRendererId rendererId_from_opt_name(const QString& name) {
    for (int i = 0; vid_map[i].name; ++i) {
        if (name == QLatin1String(vid_map[i].name))
            return vid_map[i].id;
    }
#ifndef QT_NO_OPENGL
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    return VideoRendererId_OpenGLWidget; // qglwidget is not suitable for android
#endif
    return VideoRendererId_GLWidget2;
#endif
    return VideoRendererId_Widget;
}


// #ifdef _DEBUG
//     #define new new (_NORMAL_BLOCK, __FILE__, __LINE__) // 定义new宏来替换new关键字



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



    qDebug() << aboutQtAV_PlainText();
    Config::setName(QString::fromLatin1("Player"));
    QOptions options = get_common_options();
    options.add(QString::fromLatin1("player options"))
            ("ffmpeg-log",  QString(), QString::fromLatin1("ffmpeg log level. can be: quiet, panic, fatal, error, warn, info, verbose, debug. this can override env 'QTAV_FFMPEG_LOG'"))
            ("vd-list", QString::fromLatin1("List video decoders and their properties"))
            ("-vo",
#ifndef QT_NO_OPENGL
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
             QString::fromLatin1("opengl")
#else
             QString::fromLatin1("gl")
#endif
#else
             QString::fromLatin1("qt")
#endif
             , QString::fromLatin1("video renderer engine. can be gl, qt, d2d, gdi, xv, x11."))
            ;
    options.parse(argc, argv);
    do_common_options_before_qapp(options);

    if (options.value(QString::fromLatin1("vd-list")).toBool()) {
        PropertyEditor pe;
        VideoDecoderId *vid = NULL;
        while ((vid = VideoDecoder::next(vid)) != NULL) {
            VideoDecoder *vd = VideoDecoder::create(*vid);
            pe.getProperties(vd);
            qDebug("- %s:", vd->name().toUtf8().constData());
            qDebug() << pe.buildOptions().toUtf8().constData();
        }
        exit(0);
    }
    QApplication a(argc, argv);



    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] %{message}");
    //初始化qdebug的输出重定向到文件
    qInstallMessageHandler(outLogMessageToFile);

    a.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    qDebug() <<a.arguments();
    a.setApplicationName(QString::fromLatin1("Player"));
//    a.setApplicationDisplayName(QString::fromLatin1("QtAV Player"));
    QDir::setCurrent(qApp->applicationDirPath());

    do_common_options(options);
    set_opengl_backend(options.option(QString::fromLatin1("gl")).value().toString(), a.arguments().first());
    load_qm(QStringList() << QString::fromLatin1("player"), options.value(QString::fromLatin1("language")).toString());
    QtAV::setFFmpegLogLevel(options.value(QString::fromLatin1("ffmpeg-log")).toByteArray());

    QOption op = options.option(QString::fromLatin1("vo"));
    QString vo = op.value().toString();
    if (!op.isSet()) {
        QString exe(a.arguments().at(0));
        int i = exe.lastIndexOf(QLatin1Char('-'));
        if (i > 0) {
            vo = exe.mid(i+1, exe.indexOf(QLatin1Char('.')) - i - 1);
        }
    }
    qDebug("vo: %s", vo.toUtf8().constData());
    MainWindow window;
    window.setProperty("rendererId", rendererId_from_opt_name(vo.toLower()));
    window.show();
    window.setWindowTitle(QString::fromLatin1("QtAV %1 wbsecg1@gmail.com").arg(QtAV_Version_String_Long()));
    AppEventFilter ae(&window);
    qApp->installEventFilter(&ae);

    int x = window.x();
    int y = window.y();
    op = options.option(QString::fromLatin1("width"));
    int w = op.value().toInt();
    op = options.option(QString::fromLatin1("height"));
    int h = op.value().toInt();
    op = options.option(QString::fromLatin1("x"));
    if (op.isSet())
        x = op.value().toInt();
    op = options.option(QString::fromLatin1("y"));
    if (op.isSet())
        y = op.value().toInt();
    window.resize(w, h);
    window.move(x, y);
    if (options.value(QString::fromLatin1("fullscreen")).toBool())
        window.showFullScreen();

    op = options.option(QString::fromLatin1("ao"));
    if (op.isSet()) {
        QString aos(op.value().toString());
        QStringList ao;
        if (aos.contains(QString::fromLatin1(";")))
            ao = aos.split(QString::fromLatin1(";"),
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            Qt::SkipEmptyParts
#else
            QString::SkipEmptyParts
#endif
            );
        else
            ao = aos.split(QString::fromLatin1(","),
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            Qt::SkipEmptyParts
#else
            QString::SkipEmptyParts
#endif
            );
        window.setAudioBackends(ao);
    }

    op = options.option(QString::fromLatin1("vd"));
    if (op.isSet()) {
        QStringList vd = op.value().toString().split(QString::fromLatin1(";"),
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
            Qt::SkipEmptyParts
#else
            QString::SkipEmptyParts
#endif
        );
        if (!vd.isEmpty())
            window.setVideoDecoderNames(vd);
    }
    op = options.option(QString::fromLatin1("file"));
    if (op.isSet()) {
        qDebug() << "-f set: " << op.value().toString();
        window.play(op.value().toString());
    } else {
        if (argc > 1 && !a.arguments().last().startsWith(QLatin1Char('-')) && !a.arguments().at(argc-2).startsWith(QLatin1Char('-')))
            window.play(a.arguments().last());
    }
    int ret = a.exec();
    return ret;
}

// #endif
