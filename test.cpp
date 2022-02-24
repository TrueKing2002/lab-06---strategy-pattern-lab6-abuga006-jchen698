#include "spreadsheet.cpp"

#include "gtest/gtest.h"
	
TEST(Print, oneColumnOneRow) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"Name"});
	sheet.add_row({"Diana"});
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana \n");
}

TEST(Print, oneColumnMultiRow) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"Name"});
	sheet.add_row({"Diana"});
	sheet.add_row({"George"});
	sheet.add_row({"Connor"});
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana \nGeorge \nConnor \n");
}

TEST(Print, multiColumnOneRow) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"first", "last", "age"});
	sheet.add_row({"Diana", "Stewart", "22"});
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana Stewart 22 \n");
}

TEST(Print, multiColumnMultiRow) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"first", "last", "age"});
	sheet.add_row({"Diana", "Stewart", "22"});
	sheet.add_row({"George", "Chang", "25"});
	sheet.add_row({"Connor", "Dawg", "27"});
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana Stewart 22 \nGeorge Chang 25 \nConnor Dawg 27 \n");
}

TEST(Contains, fullName) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"first", "last", "age"});
	sheet.add_row({"Diana", "Stewart", "22"});
	sheet.add_row({"George", "Chang", "25"});
	sheet.add_row({"Connor", "Dawg", "27"});
	sheet.set_selection(new Select_Contains(&sheet, "first", "Diana"));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana Stewart 22 \n");
}

TEST(Contains, partName) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"first", "last", "age"});
	sheet.add_row({"Diana", "Stewart", "22"});
	sheet.add_row({"Diane", "Chen", "24"});
	sheet.add_row({"George", "Chang", "25"});
	sheet.add_row({"Connor", "Dawg", "27"});
	sheet.set_selection(new Select_Contains(&sheet, "first", "Dian"));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana Stewart 22 \nDiane Chen 24 \n");
}

TEST(Contains, Capital) {
	std::stringstream out;
	Spreadsheet sheet;
	sheet.set_column_names({"Name"});
	sheet.add_row({"Apple"});
	sheet.add_row({"Snapple"});
	sheet.add_row({"apples"});
	sheet.add_row({"app"});
	sheet.set_selection(new Select_Contains(&sheet, "Name", "apple"));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Snapple \napples \n");
}

TEST(And, fullName) {
	std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"first", "last", "age"});
        sheet.add_row({"Diana", "Stewart", "22"});
        sheet.add_row({"George", "Chang", "25"});
        sheet.add_row({"Connor", "Dawg", "27"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "first", "Diana"), 
			new Select_Contains(&sheet, "last", "Stewart")));

	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Diana Stewart 22 \n");
}

TEST(And, PartName) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"first", "last", "age"});
        sheet.add_row({"Diana", "Stewart", "22"});
        sheet.add_row({"George", "Chang", "25"});
        sheet.add_row({"Connor", "Dawg", "27"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "first", "Ge"),
			new Select_Contains(&sheet, "age", "2")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "George Chang 25 \n");
}

TEST(And, None) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"first", "last", "age"});
        sheet.add_row({"Diana", "Stewart", "22"});
        sheet.add_row({"George", "Chang", "25"});
        sheet.add_row({"Connor", "Dawg", "27"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "first", "Diana"),
			new Select_Contains(&sheet, "age", "5")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "");
}

TEST(Not, General) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "Name", "app"),
			new Select_Not(
				new Select_Contains(&sheet, "Name", "n"))));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "apples \napp \n");
}

TEST(Not, noneTakenOut) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "Name", "app"),
			new Select_Not(
				new Select_Contains(&sheet, "Name", "A"))));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Snapple \napples \napp \n");
}

TEST(Not, None) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_And(
			new Select_Contains(&sheet, "Name", "app"),
			new Select_Not(
				new Select_Contains(&sheet, "Name", "a"))));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "");
}

TEST(Or, General) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet, "Name", "app"),
			new Select_Contains(&sheet, "Name", "A")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Apple \nSnapple \napples \napp \n");
}

TEST(Or, Less) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet, "Name", "apple"),
			new Select_Contains(&sheet, "Name", "a")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "Snapple \napples \napp \n");
}

TEST(Or, None) {
        std::stringstream out;
        Spreadsheet sheet;
        sheet.set_column_names({"Name"});
        sheet.add_row({"Apple"});
        sheet.add_row({"Snapple"});
        sheet.add_row({"apples"});
        sheet.add_row({"app"});
	sheet.set_selection(
		new Select_Or(
			new Select_Contains(&sheet, "Name", "k"),
			new Select_Contains(&sheet, "Name", "Apples")));
	sheet.print_selection(out);
	EXPECT_EQ(out.str(), "");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

