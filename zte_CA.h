#pragma once
#include "Init.h"
#include "Reve.h"


class Czte_CA
{
public:
	Czte_CA();
	~Czte_CA();
	int Begin();
	int End();

private:

	CUI* m_ui;
	CInit* m_init;
	CRever* m_rever;
	
};