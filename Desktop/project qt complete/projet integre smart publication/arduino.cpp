#include "arduino.h"

Arduino::Arduino()
{
    serial = new QSerialPort();
    arduino_is_available = false;
    arduino_port_name = "";
}

// ─────────────────────────────────────────────────────────────────────────────
//  connect_arduino()
//  Scans all available serial ports, finds the one whose VID/PID matches an
//  Arduino Uno, opens it at 9600-8N1 and returns 0 on success, -1 on failure.
// ─────────────────────────────────────────────────────────────────────────────
int Arduino::connect_arduino()
{
    // Reset state
    arduino_is_available = false;
    arduino_port_name.clear();

    // Enumerate ports and look for the Arduino Uno VID/PID
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &portInfo : ports) {
        if (portInfo.hasVendorIdentifier() && portInfo.hasProductIdentifier()) {
            if (portInfo.vendorIdentifier()  == arduino_uno_vendor_id &&
                portInfo.productIdentifier() == arduino_uno_product_id)
            {
                arduino_port_name    = portInfo.portName();
                arduino_is_available = true;
                qDebug() << "[Arduino] Found on port:" << arduino_port_name;
                break;
            }
        }
    }

    if (!arduino_is_available) {
        qDebug() << "[Arduino] Device not found. Check USB connection.";
        return -1;
    }

    // Close if already open (e.g. reconnect scenario)
    if (serial->isOpen()) {
        serial->close();
    }

    serial->setPortName(arduino_port_name);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "[Arduino] Failed to open port:" << serial->errorString();
        arduino_is_available = false;
        return -1;
    }

    // Allow the Arduino bootloader to finish resetting (it resets on DTR toggle)
    serial->setDataTerminalReady(true);

    qDebug() << "[Arduino] Connected on" << arduino_port_name;
    return 0;
}

// ─────────────────────────────────────────────────────────────────────────────
//  close_arduino()
// ─────────────────────────────────────────────────────────────────────────────
int Arduino::close_arduino()
{
    if (serial->isOpen()) {
        serial->close();
        arduino_is_available = false;
        qDebug() << "[Arduino] Port closed.";
        return 0;
    }
    return -1;   // Was already closed
}

// ─────────────────────────────────────────────────────────────────────────────
//  write_to_arduino()
// ─────────────────────────────────────────────────────────────────────────────
void Arduino::write_to_arduino(QByteArray data)
{
    if (serial->isOpen()) {
        serial->write(data);
        serial->flush();
    } else {
        qDebug() << "[Arduino] write_to_arduino: port not open.";
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  read_from_arduino()
//  Returns one complete line (including the '\n') or an empty array if there
//  is nothing available.
// ─────────────────────────────────────────────────────────────────────────────
QByteArray Arduino::read_from_arduino()
{
    if (serial->isOpen() && serial->canReadLine()) {
        data = serial->readLine();
        return data;
    }
    return QByteArray();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Accessors
// ─────────────────────────────────────────────────────────────────────────────
QSerialPort* Arduino::getserial()
{
    return serial;
}

QString Arduino::getarduino_port_name()
{
    return arduino_port_name;
}
