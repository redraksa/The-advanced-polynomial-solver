%code requires {
	typedef enum {
		TYPE_NUM,
		TYPE_ID,
		TYPE_BACKET,
		TYPE_UPLUS,
		TYPE_UMINUS
	} BoundaryType;

	typedef struct Node {
		BoundaryType left_type;
		BoundaryType right_type;
		std::string text;
	} Node;

	extern Node* create_node(const char *s);

}


%{
#include <iostream>
#include <string>
#include "Emitter.h"

extern Emitter* globalEmitter;

void yyerror(const char *s);
extern int yylex();

%}

%union {
	Node* node;
	long long value;
	std::string* s_value;
}

%destructor { delete $$; } <node>

%type <node> unar_expr expr term power primary
%token <value> NUM;
%token <s_value> ID
%token PLUS MINUS MUL DIV POWER OPENBACKET CLOSEBACKET ASSIGN SEMICOLON END 

%left PLUS MINUS
%left MUL DIV IMPLICIT_MUL
%right POWER

%%
input:
        lines END {
		YYACCEPT;
	}
	;

lines:
	lines line |
	line
	; 

line:
	ID ASSIGN unar_expr SEMICOLON {
		globalEmitter->assignID($1->c_str());
		
		delete $3;
		delete $1;
	}
        ;

unar_expr:
	PLUS expr %prec IMPLICIT_MUL {
		std::string tempStr = "+" + $2->text;
		$$ = $2;
		$$->left_type = TYPE_UPLUS;
		$$->text = tempStr;
	} |
	MINUS expr %prec IMPLICIT_MUL {
		std::string tempStr = "-" + $2->text;
		$$ = $2;
		$$->left_type = TYPE_UMINUS;
		$$->text = tempStr;

		globalEmitter->pushNumber(-1);
		globalEmitter->multiplication();
	} |
	expr {
		$$ = $1;
	}
	;

expr:
	expr PLUS term {
		std::string tempStr = $1->text + "+" + $3->text;
		$$ = $1;
		$$->right_type = $3->right_type;
		$$->text = tempStr;

		delete $3;

		globalEmitter->addition();
	} |
	expr MINUS term {
		std::string tempStr = $1->text + "-" + $3->text;
		$$ = $1;
		$$->right_type = $3->right_type;
		$$->text = tempStr;

		delete $3;

		globalEmitter->substraction();
	} |
	term {
		$$ = $1;
	}
	;
term:
	term MUL power {
		std::string tempStr = $1->text + "*" + $3->text; 
		$$ = $1; 
		$$->right_type = $3->right_type; 
		$$->text = tempStr; 

		delete $3; 

		globalEmitter->multiplication();
	} |
	term DIV power { 
		std::string tempStr = $1->text + "/" + $3->text; 
		$$ = $1; 
		$$->right_type = $3->right_type; 
		$$->text = tempStr; 

		delete $3; 

		globalEmitter->division();
	} |
	term power %prec IMPLICIT_MUL {
		if (($2->left_type == TYPE_NUM && ($1->right_type == TYPE_ID || $1->right_type == TYPE_BACKET || $1->right_type == TYPE_NUM)) ||
			($1->right_type == TYPE_ID && $2->left_type == TYPE_ID)) {
			
			std::string msg = "Invalid position: \"" + $1->text + "\" and \"" + $2->text + "\". You can't use here implicit multiplication";
			yyerror(msg.c_str());
			delete $1;
			delete $2;
			YYERROR;
		}

		std::string tempStr = $1->text + $2->text;
		$$ = $1;
		$$->right_type = $2->right_type;
		$$->text = tempStr;

		delete $2;

		globalEmitter->multiplication();

	} |
	power {
		$$ = $1;
	}
	;
power: 
	primary POWER power {
		std::string tempStr = $1->text + "^" + $3->text;
		$$ = $3;
		$$->left_type = $1->left_type;
		$$->text = tempStr;

		delete $1;

		globalEmitter->power();
	} |
	primary {
		$$ = $1;
	}
	;

primary:
	NUM {
		$$ = create_node(std::to_string($1).c_str());
		$$->left_type = TYPE_NUM;
		$$->right_type = TYPE_NUM;

		globalEmitter->pushNumber($1);
	} |
	ID {
		$$ = create_node($1->c_str());
		$$->left_type = TYPE_ID;
		$$->right_type = TYPE_ID;

		globalEmitter->pushID($1->c_str());

		delete $1;
	} |
	OPENBACKET unar_expr CLOSEBACKET {
		std::string tempStr = "(" + $2->text + ")";
		$$ = $2;
		$$->right_type = TYPE_BACKET;
		$$->left_type = TYPE_BACKET;
		$$->text = tempStr;		
	}
	;
%%

void yyerror(const char *s) {
        std::cerr << "Parser error: " << s << "\n";
}

Node* create_node(const char *s) {
	Node* newNode = new Node();
	newNode->text = std::string(s);
	return newNode;
}
