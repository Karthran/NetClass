#pragma once

enum class OperationCode
{
	STOP,
	CHECK_SIZE,
	CHECK_NAME,
	CHECK_LOGIN,
	REGISTRATION,
	SIGN_IN,
	READY,
	ERROR,
};

const size_t HEADER_SIZE = 256;
