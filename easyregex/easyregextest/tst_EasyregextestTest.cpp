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

   void testCaseErrorConditions();

   void testMaxCaptures();
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
   QTest::addColumn< QString > ("secondSubcapture");


   QTest::newRow("real world test") << ".*inet (.*) brd.*"
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << true
                                    << false
                                    << 1
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0"
                                    << "192.168.1.26/16"
                                    << "";

   QTest::newRow("should fail")     << ".*ipet (.*) brd.*"
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << false
                                    << false
                                    << 0
                                    << ""
                                    << ""
                                    << "";

   QTest::newRow("multi line")      << ".*inet (.*) brd.*"
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\n    inet 192.168.long.26/16 brd 192.168.255.255 scope global eth0\n"
                                    << true
                                    << false
                                    << 1
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0"
                                    << "192.168.1.26/16"
                                    << "";

   QTest::newRow("multi line2")     << ".*inet (.*) .*"
                                    << "sup\n    inet small stuff\n          inet hardeeharharharhar so long"
                                    << true
                                    << false
                                    << 1
                                    << "    inet small stuff"
                                    << "small"
                                    << "";

   QTest::newRow("inet mask")       << ".*inet ([0-9\\.]*)/([0-9]*) .*"
                                    << "before\n    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0\nafter\n"
                                    << true
                                    << false
                                    << 2
                                    << "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0"
                                    << "192.168.1.26"
                                    << "16";
}

void EasyregextestTest::testCaseErrorConditions()
{
   // Check all members in the following sequence

   EasyRegex re(".*inet ([0-9\\.]*)/([0-9]*) .*");

   std::string testString = "    inet 192.168.1.26/16 brd 192.168.255.255 scope global eth0";

   //**************************************************************************
   // Initial value testing
   //**************************************************************************

   // Test all the members (Initial values)
   QCOMPARE(re.captureCount(), 0);
   QCOMPARE(re.getCapture(0), std::string(""));
   QCOMPARE(re.getAllCaptures(), std::vector<std::string>());
   QCOMPARE(re.getEntireMatch(), std::string(""));
   QCOMPARE(re.isError(), true);
   QCOMPARE(re.getErrorMessage(), std::string("Need to call match first"));

   //**************************************************************************
   // Good capture testing
   //**************************************************************************

   // Do a good capture
   bool matchError = true;  // Preset to a failure condition
   bool matchSuccess = re.match(testString, &matchError);

   // Test all members
   QVERIFY(matchSuccess);
   QVERIFY(!matchError);

   QCOMPARE(re.captureCount(), 2);
   QCOMPARE(re.getCapture(0), std::string("192.168.1.26"));
   QCOMPARE(re.getCapture(1), std::string("16"));
   QCOMPARE(re.getCapture(2), std::string(""));

   std::vector<std::string> expectedCaptures;
   expectedCaptures.push_back("192.168.1.26");
   expectedCaptures.push_back("16");

   QCOMPARE(re.getAllCaptures(), expectedCaptures);
   QCOMPARE(re.getEntireMatch(), std::string(testString));
   QCOMPARE(re.isError(), false);
   QCOMPARE(re.getErrorMessage(), std::string(""));

   //**************************************************************************
   // Bad capture testing
   //**************************************************************************

   // Do a failed capture (should get compilation error)
   re.setPattern(".*inet [ .*");
   matchSuccess = re.match(testString);

   // Test all members
   QVERIFY(!matchSuccess);
   //QVERIFY(matchError);

   QCOMPARE(re.captureCount(), 0);
   QCOMPARE(re.getCapture(0), std::string(""));
   QCOMPARE(re.getAllCaptures(), std::vector<std::string>());
   QCOMPARE(re.getEntireMatch(), std::string(""));
   QCOMPARE(re.isError(), true);

   qDebug() << "Error Message: " << QString(re.getErrorMessage().c_str());

   QCOMPARE(re.getErrorMessage(), std::string("Error during regex compilation: Unmatched [ or [^"));

   //**************************************************************************
   // Good capture testing
   //**************************************************************************

   // Do a good capture
   re.setPattern(".*inet ([0-9\\.]*)/([0-9]*) .*");
   matchError = true;  // Preset to a failure condition
   matchSuccess = re.match(testString, &matchError);

   // Test all members
   QVERIFY(matchSuccess);
   QVERIFY(!matchError);

   QCOMPARE(re.captureCount(), 2);
   QCOMPARE(re.getCapture(0), std::string("192.168.1.26"));
   QCOMPARE(re.getCapture(1), std::string("16"));
   QCOMPARE(re.getCapture(2), std::string(""));
   QCOMPARE(re.getAllCaptures(), expectedCaptures);
   QCOMPARE(re.getEntireMatch(), std::string(testString));
   QCOMPARE(re.isError(), false);
   QCOMPARE(re.getErrorMessage(), std::string(""));
}

void EasyregextestTest::testMaxCaptures()
{
   EasyRegex getUpperCaseLetterGroups(".*(s.*s).*(y.*y).*(z.*z).*(g.*g).*");
   bool success = getUpperCaseLetterGroups.match("Hello s1s what y2y is z3z on g4g nothin");

   QVERIFY(success);

   qDebug() << "Entire match: " << getUpperCaseLetterGroups.getEntireMatch().c_str();
   qDebug() << "First match: " << getUpperCaseLetterGroups.getCapture(3).c_str();

   QCOMPARE(getUpperCaseLetterGroups.captureCount(), 4);
   QCOMPARE(getUpperCaseLetterGroups.getCapture(0), std::string("s1s"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(1), std::string("y2y"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(2), std::string("z3z"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(3), std::string("g4g"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(4), std::string(""));

   // Now limit matches to 2
   getUpperCaseLetterGroups.setMaxNumberOfMatches(3);

   success = getUpperCaseLetterGroups.match("Hello s1s what y2y is z3z on g4g nothin");

   QVERIFY(success);

   qDebug() << "Entire match: " << getUpperCaseLetterGroups.getEntireMatch().c_str();
   qDebug() << "First match: " << getUpperCaseLetterGroups.getCapture(3).c_str();

   QCOMPARE(getUpperCaseLetterGroups.captureCount(), 2);
   QCOMPARE(getUpperCaseLetterGroups.getCapture(0), std::string("s1s"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(1), std::string("y2y"));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(2), std::string(""));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(3), std::string(""));
   QCOMPARE(getUpperCaseLetterGroups.getCapture(4), std::string(""));

}

QTEST_APPLESS_MAIN(EasyregextestTest)

#include "tst_EasyregextestTest.moc"
