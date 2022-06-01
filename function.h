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

    RSA* createRSA(unsigned char* key, int p_public)
    { 
        RSA *rsa= NULL;
        BIO *keybio ;
        keybio = BIO_new_mem_buf(key, -1); // 읽기 전용 메모리 만들기 BIO 
        
        if (keybio==NULL)
        {
            printf( "Failed to create key BIO");
            return 0;
        } 
        
        /* PEM형식인 키 파일을 읽어와서 RSA 구조체 형식으로 변환 */
        if(p_public) // PEM public 키로 RSA 생성    
        { 
            rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
        }
        else // PEM private 키로 RSA 생성    
        {
            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
        }
        
        if(rsa == NULL)
        {
            printf( "Failed to create RSA");
        }
        
        return rsa;    
    }
    
    int private_decrypt(unsigned char * enc_data,int data_len, unsigned char * k_key, unsigned char *decrypted)
    {
        RSA * rsa = createRSA(k_key,0);
        int  result = RSA_private_decrypt(data_len,enc_data,decrypted,rsa,padding);
        return result;
    }

    string decryptData(string rsaEncrypted, string aesEncrypted)
    {
        ifstream file("certificate/private.pem");
        stringstream readPrivateKey;
        readPrivateKey << file.rdbuf();

        unsigned char decrypted[4098]={}; // 복호화한 결과를 저장할 공간 

        unsigned char decrypted[4098]={};
        int decrypted_length = private_decrypt(encrypted,encrypted_length,privateKey, decrypted);

        if(decrypted_length == -1)  // RSA_private_decrypt() returns -1 on error    
        { 
            printLastError("Private Decrypt failed ");
            exit(0);
        }
        
        printf("Decrypted Text = %s\n",decrypted);


        return privateKey.str();
    }
};
