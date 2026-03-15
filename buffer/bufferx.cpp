#include "pch.h"
#include "bufferx.h"
namespace plib // project library
{
	bufferx::bufferx(uintmax_t size)
	{
		if (size == 0x0)
			return;
		data = new (_STD nothrow) uint8_t[size];
		if (!data)
			throw _STD exception("bufferx: memory allocation failure");
		else {
			this->size = size, pos = 0x0;
			_STD memset(data, 0x0, size);
		}
	}
	bufferx::bufferx(uint8_t* blc, uintmax_t size, uintmax_t pos)
	{
		if (!blc || size == 0x0)
			throw _STD exception("bufferx: invalid parameters");
		if (data)
			delete[] data;
		this->pos = pos, this->size = size;
		data = blc; // not copying, just point to the block
	}
	bufferx::bufferx(const::std::string& path)
	{
		if (_STD filesystem::exists(path)) {
			std::fstream ss;
			ss.open(path, _STD ios::in | _STD ios::binary);
			if (!ss.is_open())
				throw _STD exception("bufferx: failed to open file");
			ss.seekg(0, _STD ios::end);
			this->size = (uintmax_t)ss.tellg();
			ss.seekg(0, _STD ios::beg);
			data = new (_STD nothrow) uint8_t[this->size];
			if (!data) {
				ss.close();
				throw _STD exception("bufferx: memory allocation failure");
			} else {
				ss.read(reinterpret_cast<char*>(data), this->size);
				ss.close();
				this->pos = 0x0;
			}
		} else throw _STD exception("bufferx: file does not exist");
	}
	bufferx::~bufferx()
	{
		if (auto_cleanup == true) {
			if (data) {
				delete[] data;
				data = nullptr;
			} this->size = 0x0, this->pos = 0x0;
		}
	}
	const uintmax_t bufferx::get_size() const
	{
		return this->size;
	}
	const uintmax_t bufferx::get_pos() const
	{
		return this->pos;
	}

	void bufferx::set_string(const::std::string& value)
	{
		size_t estimate = sizeof(uint16_t) + value.length();
		if (this->pos + estimate > this->size)
			if (auto_resize)
				this->resize(this->size + estimate);
			else throw _STD exception("bufferx: insufficient buffer size for string");
		uint16_t str_len = (uint16_t)value.length();
		_STD memcpy(this->data + this->pos, &str_len, sizeof(uint16_t));
		this->pos += sizeof(uint16_t);
		_STD memcpy(this->data + this->pos, value.data(), str_len);
		this->pos += str_len;
	}

	void bufferx::set_string(const char* value)
	{
		size_t estimate = sizeof(uint16_t) + _STD strlen(value);
		if (this->pos + estimate > this->size)
			if (auto_resize)
				this->resize(this->size + estimate);
			else throw _STD exception("bufferx: insufficient buffer size for string");
		uint16_t str_len = (uint16_t)_STD strlen(value);
		_STD memcpy(this->data + this->pos, &str_len, sizeof(uint16_t));
		this->pos += sizeof(uint16_t);
		_STD memcpy(this->data + this->pos, value, str_len);
		this->pos += str_len;
	}

	void bufferx::set_ubyte(const uint8_t& value)
	{
		if (this->pos + sizeof(uint8_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for ubyte");
		_STD memcpy(this->data + this->pos, &value, sizeof(uint8_t));
		this->pos += sizeof(uint8_t);
	}

	void bufferx::set_ushort(const uint16_t& value)
	{
		if (this->pos + sizeof(uint16_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for ushort");
		_STD memcpy(this->data + this->pos, &value, sizeof(uint16_t));
		this->pos += sizeof(uint16_t);
	}

	void bufferx::set_uint(const uint32_t& value)
	{
		if (this->pos + sizeof(uint32_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for uint");
		_STD memcpy(this->data + this->pos, &value, sizeof(uint32_t));
		this->pos += sizeof(uint32_t);
	}

	void bufferx::set_ulong(const uint64_t& value)
	{
		if (this->pos + sizeof(uint64_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for ulong");
		_STD memcpy(this->data + this->pos, &value, sizeof(uint64_t));
		this->pos += sizeof(uint64_t);
	}

	void bufferx::set_byte(const int8_t& value)
	{
		if (this->pos + sizeof(int8_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for byte");
		_STD memcpy(this->data + this->pos, &value, sizeof(int8_t));
		this->pos += sizeof(int8_t);
	}

	void bufferx::set_short(const int16_t& value)
	{
		if (this->pos + sizeof(int16_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for short");
		_STD memcpy(this->data + this->pos, &value, sizeof(int16_t));
		this->pos += sizeof(int16_t);
	}

	void bufferx::set_int(const int32_t& value)
	{
		if (this->pos + sizeof(int32_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for int");
		_STD memcpy(this->data + this->pos, &value, sizeof(int32_t));
		this->pos += sizeof(int32_t);
	}

	void bufferx::set_long(const int64_t& value)
	{
		if (this->pos + sizeof(int64_t) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for long");
		_STD memcpy(this->data + this->pos, &value, sizeof(int64_t));
		this->pos += sizeof(int64_t);
	}

	void bufferx::set_float(const float& value)
	{
		if (this->pos + sizeof(float) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for float");
		_STD memcpy(this->data + this->pos, &value, sizeof(float));
		this->pos += sizeof(float);
	}

	void bufferx::set_double(const double& value)
	{
		if (this->pos + sizeof(double) > this->size)
			if (auto_resize)
				this->resize(this->size + sizeof(uint8_t));
			else throw _STD exception("bufferx: insufficient buffer size for double");
		_STD memcpy(this->data + this->pos, &value, sizeof(double));
		this->pos += sizeof(double);
	}

	const uint8_t bufferx::get_ubyte()
	{
		uint8_t num = *(reinterpret_cast<uint8_t*>(this->data + this->pos));
		this->pos += sizeof(uint8_t);
		return num;
	}

	const uint16_t bufferx::get_ushort()
	{
		uint16_t num = *(reinterpret_cast<uint16_t*>(this->data + this->pos));
		this->pos += sizeof(uint16_t);
		return num;
	}

	const uint32_t bufferx::get_uint()
	{
		uint32_t num = *(reinterpret_cast<uint32_t*>(this->data + this->pos));
		this->pos += sizeof(uint32_t);
		return num;
	}
	const uint64_t bufferx::get_ulong()
	{
		uint64_t num = *(reinterpret_cast<uint64_t*>(this->data + this->pos));
		this->pos += sizeof(uint64_t);
		return num;
	}

	const int8_t bufferx::get_byte()
	{
		int8_t num = *(reinterpret_cast<int8_t*>(this->data + this->pos));
		this->pos += sizeof(int8_t);
		return num;
	}
	const int16_t bufferx::get_short()
	{
		int16_t num = *(reinterpret_cast<int16_t*>(this->data + this->pos));
		this->pos += sizeof(int16_t);
		return num;
	}
	const int32_t bufferx::get_int()
	{
		int32_t num = *(reinterpret_cast<int32_t*>(this->data + this->pos));
		this->pos += sizeof(int32_t);
		return num;
	}
	const int64_t bufferx::get_long()
	{
		int64_t num = *(reinterpret_cast<int64_t*>(this->data + this->pos));
		this->pos += sizeof(int64_t);
		return num;
	}
	const float bufferx::get_float()
	{
		float num = *(reinterpret_cast<float*>(this->data + this->pos));
		this->pos += sizeof(float);
		return num;
	}
	const double bufferx::get_double()
	{
		double num = *(reinterpret_cast<double*>(this->data + this->pos));
		this->pos += sizeof(double);
		return num;
	}
	const std::string bufferx::get_string()
	{
		uint16_t lens = *(reinterpret_cast<int16_t*>(this->data + this->pos));
		pos += sizeof(uint16_t);
		_STD string str = std::string{ (const char*)data + this->pos, lens };
		this->pos += lens;
		return str;
	}
	void bufferx::resize(const uintmax_t& size)
	{
		if (size == 0x0)
			throw _STD exception("bufferx: invalid resize size");
		uint8_t* new_data = new (_STD nothrow) uint8_t[size];
		if (!new_data)
			throw _STD exception("bufferx: memory allocation failure during resize");
		else {
			_STD memset(new_data, 0x0, size);
			uintmax_t copy_size = (size < this->size) ? size : this->size;
			_STD memcpy(new_data, this->data, copy_size);
			if (this->data)
				delete[] this->data;
			this->data = new_data;
			this->size = size;
			if (this->pos > this->size)
				this->pos = this->size;
		}
	}
	bool bufferx::is_enough(const uintmax_t& size)
	{
		if (this->pos + size > this->size)
			return false;
		return true;
	}

	void bufferx::to_file(const::std::string& path) const
	{
		FILE* fp = _STD fopen(path.c_str(), "wb");
		if (!fp)
			throw _STD exception("bufferx: failed to open file for writing");
		size_t written = _STD fwrite(this->data, 1, this->size, fp);
		if (written != this->size) {
			_STD fclose(fp);
			throw _STD exception("bufferx: failed to write all data to file");
		}
		_STD fflush(fp);
		_STD fclose(fp);
	}

	bufferx* bufferx::from_file(const::std::string& path) const
	{
		std::ifstream ifs(path, std::ios::binary);
		if (!ifs.is_open())
			throw _STD exception("bufferx: failed to open file for reading");
		ifs.seekg(0, std::ios::end);
		uintmax_t file_size = (uintmax_t)ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		uint8_t* file_data = new (_STD nothrow) uint8_t[file_size];
		if (!file_data) {
			ifs.close();
			throw _STD exception("bufferx: memory allocation failure during file read");
		} else {
			ifs.read(reinterpret_cast<char*>(file_data), file_size);
			ifs.close();
			return new bufferx(file_data, file_size);
		}
		return nullptr;
	}

	void bufferx::set_resources(const uint8_t& auto_resize, const uint8_t& auto_cleanup)
	{
		this->auto_resize = (auto_resize == plib::auto_resize ? true : false);
		this->auto_cleanup = (auto_cleanup == plib::auto_cleanup ? true : false);
	}

}