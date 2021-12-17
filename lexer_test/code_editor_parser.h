#ifndef CODE_EDITOR_PARSER_H
#define CODE_EDITOR_PARSER_H

#include"lexertk/lexertk.hpp"
/*
 * Base class for a syntax
 * This class is supposed to associate lexertk tokens to colors.
 * It contains keywords for a syntax, as well as
 *
 *
 * If I don't want to parse the whole file all the time, I can make a string_view that represents the displayable text based on the number of \n
*/

struct color_token : public token
{


};

class code_editor_syntax
{
public:


private:

};

class code_editor_painter_base
{
public:

private:

};


#endif // CODE_EDITOR_PARSER_H
