#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>
#include <iostream>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    Select_Column() {}

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains: public Select_Column
{
private:
    std::string str;
public:
    Select_Contains(const Spreadsheet* sheet, const std::string& name, const std::string& str)
    {
	column = sheet->get_column_by_name(name);
        if(column == -1) std::cout << "ERROR: INVALID COLUMN NAME" << std::endl;
	this->str = str;
    }

    virtual bool select(const std::string& s) const
    {
	if(s.find(str, 0) != std::string::npos)
	    return true;
	return false;
    }
};

class Select_Not: public Select
{
private:
    Select* selection;
public:
    Select_Not(Select* selection)
    {
	this->selection = selection;	
    }

    ~Select_Not() 
    {
	delete selection;
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
	return !selection->select(sheet, row);
    }
};

class Select_And: public Select
{
private:
    Select* left;
    Select* right;
public:
    Select_And(Select* left, Select* right)
    {
	this->left = left;
	this->right = right;
    }
	
    ~Select_And()
    {
	delete left;
	delete right;
    }
 
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return left->select(sheet, row) && right->select(sheet, row);
    }
};

class Select_Or: public Select
{
private:
    Select* left;
    Select* right;
public:
    Select_Or(Select* left, Select* right)
    {
        this->left = left;
        this->right = right;
    }

    ~Select_Or()
    {
	delete left;
	delete right;
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return left->select(sheet, row) || right->select(sheet, row);
    }
};
#endif //__SELECT_HPP__
