#include <iostream>
#include <string>
#include <openssl/err.h>

#ifndef PKI_BASE_H
#define PKI_BASE_H

class pki_base
{
    protected:
	string desc;
	string error;
	bool openssl_error();
    public:
	pki_base(const string d);
	pki_base();
	virtual void fromData(unsigned char *p, int size){
		cerr << "VIRTUAL FUNCTION CALLED: fromData\n"; };
	virtual unsigned char *toData(int *size){
		cerr << "VIRTUAL FUNCTION CALLED: toData\n";
		return NULL;};
	virtual bool compare(pki_base *ref){
		cerr << "VIRTUAL FUNCTION CALLED: compare\n";
		return false;};
	virtual ~pki_base();
        string getDescription();
        void setDescription(const string d );
	string getError();
};

#endif