// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

//Материал взят из конспектов лекции Сысоева А.В. на 19.09.22

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0)
		throw exception("len must exist");

	BitLen = len;
	MemLen = BitLen / (sizeof(TELEM) * 8) + 1;
	pMem = new TELEM[MemLen];
	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const  // битовая маска для бита n
{
	return 1 << (n & (sizeof(TELEM) * 8) - 1);
}


//--------------------------------------------------------------------------------------------------------
// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < BitLen && n >= 0)
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw exception("bit position is out of range");
	
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < BitLen && n >= 0)
		pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
	else throw exception("bit position is out of range");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < BitLen && n >= 0)
	{
		TELEM gb = pMem[GetMemIndex(n)] & GetMemMask(n);
		if (gb==0)  
			return 0;
		else return 1;
	}
		
	else throw exception("bit position is out of range");
}


//--------------------------------------------------------------------------------------------------------
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen = bf.BitLen;

	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		TELEM* p = new TELEM[bf.MemLen];
		delete[] pMem;
		pMem = p;
	}
	memcpy(pMem, bf.pMem, MemLen*sizeof(TELEM));
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (size_t i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (*this == bf) return 0;
	return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (BitLen > bf.BitLen)
	{
		TBitField tmp(*this);
		for (size_t i = 0; i < bf.MemLen; i++)
			tmp.pMem[i] = pMem[i] | bf.pMem[i];
		return tmp;
	}
	else
	{
		TBitField tmp(bf);
		for (size_t i = 0; i < MemLen; i++)
			tmp.pMem[i] = pMem[i] | bf.pMem[i];
		return tmp;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (BitLen > bf.BitLen)
	{
		TBitField tmp(*this);
		for (size_t i = 0; i < bf.MemLen; i++)
			tmp.pMem[i] = pMem[i] & bf.pMem[i];
		return tmp;
	}
	else
	{
		TBitField tmp(bf);
		for (size_t i = 0; i < MemLen; i++)
			tmp.pMem[i] = pMem[i] & bf.pMem[i];
		return tmp;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (size_t i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];

	tmp.pMem[MemLen - 1] &= GetMemMask(BitLen) - 1;
	return tmp;
}


//--------------------------------------------------------------------------------------------------------
// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	size_t i;
	istr >> i;
	bf.SetBit(i);
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	const int len=bf.GetLength();
	for (size_t i = 0; i < len; i++)
	{
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	}
	return ostr;
}
