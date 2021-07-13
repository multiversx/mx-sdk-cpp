#include "filehandler/keyfilereader.h"
#include "cryptosignwrapper.h"
#include <sodium.h>
#include "errors.h"
#include "base64.h"
#include "hex.h"
#include "aes_128_ctr/aes.hpp"
#include "aes_128_ctr/aes.h"
#include "aes_128_ctr/aes.c"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "json/json.hpp"

KeyFileReader::KeyFileReader(std::string const &filePath) :
        IFile(filePath)
{
    try
    {
        KeyFileReader::checkFile();

        m_encryptedData = getFileContent();
    }
    catch (std::exception const &error)
    {
        throw;
    }
}

void KeyFileReader::checkFile() const
{
    if (!IFile::fileExists())
    {
        throw std::invalid_argument(ERROR_MSG_FILE_DOES_NOT_EXIST + getFilePath());
    }
    if (!IFile::isFileExtension("json"))
    {
        throw std::invalid_argument(ERROR_MSG_FILE_EXTENSION_INVALID);
    }
}

Address KeyFileReader::getAddress() const
{
    return Address(m_encryptedData.address);
}

bytes KeyFileReader::getSeed() const
{
    return bytes();
}

EncryptedData KeyFileReader::getFileContent() const
{
    EncryptedData ret;

    try
    {
        std::ifstream stream(IFile::getFilePath());

        nlohmann::json data = nlohmann::json::parse(stream);

        ret.address = data["address"];
        ret.bech32 = data["bech32"];
        ret.version = data["version"];
        ret.id = data["id"];
        ret.cipher = data["crypto"]["cipher"];
        ret.cipherText = data["crypto"]["ciphertext"];
        ret.iv = data["crypto"]["cipherparams"]["iv"];
        ret.cipher = data["crypto"]["cipher"];
        ret.kdf = data["crypto"]["kdf"];
        ret.kdfparams.dklen = data["crypto"]["kdfparams"]["dklen"];
        ret.kdfparams.n = data["crypto"]["kdfparams"]["n"];
        ret.kdfparams.r = data["crypto"]["kdfparams"]["r"];
        ret.kdfparams.p = data["crypto"]["kdfparams"]["p"];
        ret.kdfparams.salt = data["crypto"]["kdfparams"]["salt"];
        ret.mac = data["crypto"]["mac"];

        //decrypt
        std::string passw = "12345678Qq!";

        ret.kdfparams.salt = util::hexToString(ret.kdfparams.salt);

        ret.cipherText = util::hexToString(ret.cipherText);

        bytes pass(passw.begin(), passw.end());
        bytes salt(ret.kdfparams.salt.begin(),
                   ret.kdfparams.salt.end());


        uint8_t derivedKey[32];
        crypto_pwhash_scryptsalsa208sha256_ll
        (pass.data(),passw.size(),
         salt.data(),salt.size(),
         ret.kdfparams.n,8,ret.kdfparams.p,
         derivedKey,32);

        std::string derivedKeystr;
        for (int i= 0 ; i<32;i++)
        {
            derivedKeystr.push_back(derivedKey[i]);
        }

        bytes secondHalf(derivedKeystr.begin() + 16, derivedKeystr.end());
        bytes firstHalf(derivedKeystr.begin(), derivedKeystr.begin() + 16);

        std::cerr<<util::stringToHex(derivedKeystr) << "\n";

        unsigned char mac[32];



        crypto_auth_hmacsha256_state state;

        crypto_auth_hmacsha256_init(&state, secondHalf.data(), secondHalf.size());



        unsigned char msggg[ret.cipherText.size()];
        for (int i =0;i<ret.cipherText.size();i++)
        {
            msggg[i] = ret.cipherText[i];
        }
        crypto_auth_hmacsha256_update(&state, msggg,ret.cipherText.size());

        crypto_auth_hmacsha256_final(&state,mac);






        std::string macStr;
        for (int i= 0 ; i<32;i++)
        {
            macStr.push_back(mac[i]);
        }
        std::cerr<<util::stringToHex(macStr) << "\n";


















        ret.iv = util::hexToString(ret.iv);

        unsigned char iv[ret.iv.size()];
        for (int i=0;i<ret.iv.size();i++)
        {
            iv[i] = ret.iv[i];
        }



        unsigned char cipherText2[ret.cipherText.size()];
        for (int i =0;i<ret.cipherText.size();i++)
        {
            cipherText2[i] = ret.cipherText[i];
        }


        AES_ctx ctx;


        AES_init_ctx_iv(&ctx, firstHalf.data(),iv);


        AES_CTR_xcrypt_buffer(&ctx,cipherText2, ret.cipherText.size());

        std::string secretKey;
        for (int i=0;i<ret.cipherText.size();i++)
        {
            secretKey.push_back(cipherText2[i]);
        }


        std::cerr<<util::stringToHex(secretKey)<< "\n";


        bytes sk(secretKey.begin(), secretKey.end());
        bytes pk = wrapper::crypto::getPublicKey(sk);

        std::string pkStr (pk.begin(), pk.end());
        std::cerr<<util::stringToHex(pkStr);

    }
    catch(...)
    {
        throw;
    }

    return ret;
}

