#include <QtTest>
#include <QtDebug>
#include <QString>
#include "EasyRegex.h"



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
   qDebug() << __PRETTY_FUNCTION__;
}

void EasyregextestTest::initTestCase()
{
   qDebug() << __PRETTY_FUNCTION__;
}

void EasyregextestTest::cleanupTestCase()
{
}

void EasyregextestTest::testCase1()
{
   QFETCH(QString, pattern);
   QFETCH(QString, haystack);
   QFETCH(bool, matchSuccess);
   QFETCH(bool, hasError);
   QFETCH(int, numCaptures);
   QFETCH(QString, entireCapture);
   QFETCH(QString, firstSubcapture);

   EasyRegex re(pattern.toStdString());

   bool success = re.match(haystack.toStdString());

   // QCOMPARE(actual, expected)
   QCOMPARE(success, matchSuccess);
   QCOMPARE(re.isError(), hasError);
   QCOMPARE(re.captureCount(), numCaptures);

   QString em(re.getEntireMatch().c_str());
   QCOMPARE(em, entireCapture);

   QString firstMatch(re.getCapture(0).c_str());
   QCOMPARE(firstMatch, firstSubcapture);


}

void EasyregextestTest::testCase1_data()
{
   QTest::addColumn< QString >("pattern");
   QTest::addColumn< QString >("haystack");
   QTest::addColumn< bool > ("matchSuccess");
   QTest::addColumn< bool > ("hasError");
   QTest::addColumn< int > ("numCaptures");
   QTest::addColumn< QString > ("entireCapture");
   QTest::addColumn< QString > ("firstSubcapture");

   QTest::newRow("real world test") << ".*inet (.*)"
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << true
                                    << false
                                    << 1
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << "192.168.1.26/16 brd 192.168.255.255 scope global eth0\n";

   QTest::newRow("should fail")     << ".*ipet (.*)"
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << false
                                    << false
                                    << 0
                                    << ""
                                    << "";
}

QTEST_APPLESS_MAIN(EasyregextestTest)

#include "tst_EasyregextestTest.moc"
