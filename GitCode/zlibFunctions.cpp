#include "zlibFunctions.h"

bool compress(std::string& in, std::string& out)
{
	z_stream zs{};
	if (deflateInit(&zs, Z_DEFAULT_COMPRESSION) != Z_OK) {
		std::cerr << "zlib not initialized.\n";
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(in.data());
	zs.avail_in = in.size();

	char outBuffer[4096];

	int ret;
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outBuffer);
		zs.avail_out = sizeof(outBuffer);

		ret = deflate(&zs, Z_FINISH);

		if (ret != Z_OK && ret != Z_STREAM_END) {
			std::cerr << "Compression failed (zlib error: " << ret << ")\n";
			deflateEnd(&zs);
			return false;
		}

		size_t bytesProduced = sizeof(outBuffer) - zs.avail_out;
		out.insert(out.end(), outBuffer, outBuffer + bytesProduced);

	} while (ret != Z_STREAM_END);

	deflateEnd(&zs);

	return true;
}

bool decompress(std::string& in, std::string& out)
{
	z_stream zs = {};
	if (inflateInit(&zs) != Z_OK) {
		std::cerr << "zlib not initialized.\n";
		return false;
	}

	zs.next_in = reinterpret_cast<Bytef*>(in.data());
	zs.avail_in = in.size();

	char outBuffer[4096]; // 4kb;

	int ret;
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outBuffer);
		zs.avail_out = sizeof(outBuffer);

		ret = inflate(&zs, 0);

		if (ret != Z_OK && ret != Z_STREAM_END) {
			std::cerr << "Decompression failed (zlib error: " << ret << ")\n";
			inflateEnd(&zs);
			return false;
		}

		size_t bytesProduced = sizeof(outBuffer) - zs.avail_out;
		out.insert(out.end(), outBuffer, outBuffer + bytesProduced);

	} while (ret != Z_STREAM_END);

	inflateEnd(&zs);

	return true;
}