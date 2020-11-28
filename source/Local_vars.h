#ifndef LV_H
#define LV_H





#include "PrimitiveTypes2.h"
#include <unordered_set>
#include <string>
#include <unordered_map>


class Local_vars
{
public:
	/*! User defined symbols for vectors*/
	std::unordered_set<std::wstring> m_vs;
	/*! User defined symbols for scalars*/
	std::unordered_set<std::wstring> m_ss;
    /*! User defined variables containing expressions*/
	std::unordered_map<std::wstring, std::unique_ptr<Unit>> m_lv;
};

#endif // LV_H
