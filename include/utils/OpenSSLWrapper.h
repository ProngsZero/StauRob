#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport)
		const char* GetOpenSSLVersion();

	__declspec(dllexport)
		bool GetOpenSSLEVPHash(const char* input, char* output, int outputSize);

	__declspec(dllexport)
		const char* GenerateSelfSignedCert();

#ifdef __cplusplus
}
#endif
