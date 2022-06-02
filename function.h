#pragma once
#include "crow.h"
#include <fstream>
#include <string>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <iostream>
using namespace std;

class Function
{

public:
    tuple<string, string, string> requestParser(crow::json::rvalue request)
    {   
        string rsaEncrypted = request["encKey"].s();
        string aesEncrypted = request["certData"].s();
        string pinId = request["pinId"].s();
        
        return {rsaEncrypted, aesEncrypted, pinId};
    }

    int decryptRsaWithPrivateKey(unsigned char* inData, int inLen, unsigned char* outData)
    {
        std::ifstream file("private.pem");
        std::stringstream privateKey;
        privateKey << file.rdbuf();
        std::string privateKeyData = privateKey.str();
        char privateKeyText[strlen(privateKeyData.c_str())]; 
        strcpy(privateKeyText, privateKeyData.c_str());
        BIO* bio = BIO_new_mem_buf(privateKeyText, -1);

        RSA* rsaPrivateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);

        BIO_free(bio);

        int len = RSA_private_decrypt(inLen, (const unsigned char*)inData, outData, rsaPrivateKey, RSA_PKCS1_OAEP_PADDING);

        return len;
    }

    string decryptData(string rsaEncryptedstr, string aesEncrypted)
    {
        char decData[512] = { 0, };
        char rsaEncrypted[strlen(rsaEncryptedstr.c_str())];
        strcpy(rsaEncrypted, rsaEncryptedstr.c_str());
        int len = decryptRsaWithPrivateKey((unsigned char *)rsaEncrypted, strlen(rsaEncrypted), (unsigned char *)decData);
        
        printf("dec : %s\n", decData);
    }
};
