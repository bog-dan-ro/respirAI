#include <QtTest>

#include <packet.h>

// add necessary includes here

class PacketTest : public QObject
{
    Q_OBJECT

public:
    PacketTest();
    ~PacketTest();

private slots:
    void test_case1();

};

PacketTest::PacketTest()
{

}

PacketTest::~PacketTest()
{

}

void PacketTest::test_case1()
{

}

QTEST_APPLESS_MAIN(PacketTest)

#include "tst_packet.moc"
