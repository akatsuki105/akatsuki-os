#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static int dec2asc(char *str, int dec)
{
	int len = 0, len_buf; //桁数
	int buf[10];
	while (1)
	{ //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
		buf[len++] = dec % 10;
		if (dec < 10)
			break;
		dec /= 10;
	}
	len_buf = len;
	while (len)
	{
		*(str++) = buf[--len] + 0x30;
	}
	return len_buf;
}

//16進数からASCIIコードに変換
static int hex2asc(char *str, int dec)
{						  //10で割れた回数（つまり桁数）をlenに、各桁をbufに格納
	int len = 0, len_buf; //桁数
	int buf[10];
	while (1)
	{
		buf[len++] = dec % 16;
		if (dec < 16)
			break;
		dec /= 16;
	}
	len_buf = len;
	while (len)
	{
		len--;
		*(str++) = (buf[len] < 10) ? (buf[len] + 0x30) : (buf[len] - 9 + 0x60);
	}
	return len_buf;
}

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}

int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		} else if (*format == 'd') {
			format++;
			const char *str;
			const int tmp = va_arg(parameters, int);
			sprintf(str, "%d", tmp);
			size_t len = strlen(str);
			if (maxrem < len) { return -1; }
			if (!print(str, len)) { return -1; }
			written += len;
		} else if (*format == 'x') {
			format++;
			const char *str;
			const int tmp = va_arg(parameters, int);
			sprintf(str, "%x", tmp);
			size_t len = strlen(str);
			if (maxrem < len) { return -1; }
			if (!print(str, len)) { return -1; }
			written += len;
		} else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}
