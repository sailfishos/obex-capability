/**
 * @file obex-capability.cpp
 * @copyright 2013 Jolla Ltd.
 * @author Bernd Wachter <bwachter@lart.info>
 * @date 2013
 */

#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include <deviceinfo.h>

#include "obex-capability.h"

ObexCapability::ObexCapability()
{
    doc = new QDomDocument("Capability SYSTEM \"obex-capability.dtd\"");
    QDomProcessingInstruction inst = doc->createProcessingInstruction("xml", "version=\"1.0\"");
    doc->appendChild(inst);

    root = doc->createElement("Capability");
    root.setAttribute("Version", "1.0");
    doc->appendChild(root);
}

QDomElement ObexCapability::appendElement(QDomElement &parent, const QString &element, const QString &text)
{
    QDomElement e = doc->createElement(element);
    parent.appendChild(e);
    QDomText t = doc->createTextNode(text);
    e.appendChild(t);

    return e;
}

QDomElement ObexCapability::appendChildren(QDomElement &parent, const QHash<QString, QString> &children)
{
    QHashIterator<QString, QString> i(children);
    while (i.hasNext()) {
        i.next();
        QDomElement c = doc->createElement(i.key());
        parent.appendChild(c);

        QDomText t = doc->createTextNode(i.value());
        c.appendChild(t);
    }

    return parent;
}

QDomElement ObexCapability::appendElementWithChildren(QDomElement &parent, const QString &element,
                                                      const QHash<QString, QString> &children)
{
    QDomElement e = doc->createElement(element);
    parent.appendChild(e);

    appendChildren(e, children);

    return e;
}

void ObexCapability::getDeviceInfo()
{
    DeviceInfo info(true);

    QDomElement p = doc->createElement("General");
    root.appendChild(p);

    appendElement(p, "Manufacturer", info.manufacturer());
    appendElement(p, "Model", info.prettyName());

    //appendElement(p, "Language", info.currentLanguage());
    appendElement(p, "SN", info.deviceUid());

    QString version = info.osVersion();
    QStringList versionBits = version.split(".");

    // report the full version
    QDomElement sw = doc->createElement("SW");
    p.appendChild(sw);
    sw.setAttribute("version", version);

    // report OS version with only major.minor
    QStringList osBits = info.designation().split(" ");
    QDomElement os = doc->createElement("OS");
    p.appendChild(os);
    os.setAttribute("id", osBits[0]);
    os.setAttribute("version", versionBits[0] + "." + versionBits[1]);

    // add: version
}

void ObexCapability::getSyncMLServices()
{
    // TODO: read the supported syncml modules from device configuration
    QHash<QString, QString> h;

    QDomElement p = doc->createElement("Service");
    root.appendChild(p);

    // add the basic syncml configuration
    h["Name"] = "SyncML";
    h["UUID"] = "SYNCML-SYNC";
    h["Version"] = "1.2";

    appendChildren(p, h);

    h.clear();
    h["Type"] = "application/vnd.syncml+wbxml";
    QDomElement o = appendElementWithChildren(p, "Object", h);

    h.clear();
    h["XNam"] = "ServerAlertedNotificationType";
    h["XVal"] = "application/vnd.syncml.ds.notification";
    appendElementWithChildren(o, "Ext", h);
}

void ObexCapability::run()
{
    QTextStream qout(stdout);
    // first get general-section
    // in separate function, try to figure out which service sections we need

    getDeviceInfo();
    getSyncMLServices();

    qout << doc->toString() << endl;
    qout.flush();
    QCoreApplication::quit();
}

int main (int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    ObexCapability mw;

    QTimer::singleShot(0, &mw, SLOT(run()));
    return app.exec();
}
