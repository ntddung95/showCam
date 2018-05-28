/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PTZController.h
 * Author: ha
 *
 * Created on August 23, 2017, 11:14 AM
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>

using namespace std;

class IPConfig{
private:
    string cameraToken = "MainStream";
    string camUrl = "";
    static size_t CallbackFunc(void *contents, size_t size, size_t nmemb, std::string *s)
    {
        size_t newLength = size*nmemb;
        size_t oldLength = s->size();
        try
        {
            s->resize(oldLength + newLength);
        }
        catch(std::bad_alloc &e)
        {
            //handle memory problem
            return 0;
        }

        std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
        return size*nmemb;
    }
public:    
    IPConfig(string ip)
    {camUrl = "http://" + ip + "/onvif/device_service";}
    
    string post(string content){
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);


            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();

        return respondContent;
    }

    string getToken(){
        std::string respondContent = "";
        CURL*curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, camUrl.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackFunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respondContent);
            string strGetProfile = "<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://www.w3.org/2005/08/addressing\"><s:Header><Security s:mustUnderstand=\"1\" xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\"><UsernameToken><Username>admin</Username><Password Type=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest\">admin</Password><Nonce EncodingType=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-soap-message-security-1.0#Base64Binary\"></Nonce><Created xmlns=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\"></Created></UsernameToken></Security></s:Header><s:Body xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"><GetProfiles xmlns=\"http://www.onvif.org/ver10/media/wsdl\"/></s:Body> </s:Envelope>";
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strGetProfile.c_str());
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            //std::cout<<"Returned:"<<respondContent;
        }
        return "";
    }
    
    enum Dir{
        Left,Right,Up,Down
    };

    void setGOP(int gop){
        string command = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:soap=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\" xmlns:tt=\"http://www.onvif.org/ver10/schema\"><soap:Body><trt:SetVideoEncoderConfiguration><trt:Configuration token=\"VideoEncoderConfiguration0\"><tt:Name>VideoEncoderConfiguration0</tt:Name><tt:UseCount>1</tt:UseCount><tt:Encoding>H264</tt:Encoding><tt:Resolution><tt:Width>1920</tt:Width><tt:Height>1080</tt:Height></tt:Resolution><tt:Quality>4</tt:Quality><tt:H264><tt:GovLength>" + MyIntToString(gop) + "</tt:GovLength><tt:H264Profile>Baseline</tt:H264Profile></tt:H264><tt:Multicast><tt:Address><tt:Type>IPv4</tt:Type><tt:IPv4Address>255.255.255.255</tt:IPv4Address></tt:Address><tt:Port>1234</tt:Port><tt:TTL>1</tt:TTL><tt:AutoStart>true</tt:AutoStart></tt:Multicast><tt:SessionTimeout>PT1H1M1S</tt:SessionTimeout></trt:Configuration><trt:ForcePersistence>true</trt:ForcePersistence></trt:SetVideoEncoderConfiguration></soap:Body></soap:Envelope>";
        post(command);
    }

    void resetWDR(){
        
    }
    std::string MyIntToString ( int Number )
    {
      std::ostringstream ss;
      ss << Number;
      return ss.str();
    }

};


#endif /* PTZCONTROLLER_H */

