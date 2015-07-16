#include <QString>
#include <QtTest>

class EasyregextestTest : public QObject
{
   Q_OBJECT

public:
   EasyregextestTest();

private Q_SLOTS:
   void initTestCase();
   void cleanupTestCase();
   void testCase1();
   void testCase1_data();
};

EasyregextestTest::EasyregextestTest()
{
}

void EasyregextestTest::initTestCase()
{
}

void EasyregextestTest::cleanupTestCase()
{
}

void EasyregextestTest::testCase1()
{
   QFETCH(QString, data);
   QVERIFY2(true, "Failure");
}

void EasyregextestTest::testCase1_data()
{
   QTest::addColumn<QString>("data");
   QTest::newRow("0") << QString();
}

QTEST_APPLESS_MAIN(EasyregextestTest)

#include "tst_EasyregextestTest.moc"
