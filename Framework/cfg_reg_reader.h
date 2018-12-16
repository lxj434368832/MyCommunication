#pragma once

#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <wtypes.h>

class cfg_reg_reader
{
private:
	// ע������
	HKEY								reg_key_root_;
	std::string							reg_item_name_;			// ע����е���
	std::vector<std::string>			reg_subkey_list_;		// ����������ı�
	std::map<std::string, std::string>	reg_key_value_list_;	// �������Ӧ��key��value
	std::recursive_mutex				reg_list_lock_;			// ������

																// �����ļ����
	std::string							cfg_file_name_;
	std::map<std::string, std::map<std::string, std::string>> cfg_section_list_;
	std::string							cfg_cur_section_;
	std::recursive_mutex				cfg_list_lock_;

public:
	cfg_reg_reader() = default;
	~cfg_reg_reader() = default;

	bool read_from_reg(HKEY reg_key_root, std::string item_name);
	bool read_from_cfg(std::string file_name);

	std::string get_value_from_reg(std::string key);
	std::string get_value_from_cfg(std::string section_name, std::string key_name);
	std::string get_value_from_cfg(std::string section_name, std::string key_name, const std::string& defaut_value);
	static bool read_str_from_cfg(std::string file_name, std::string type, std::string key, char *out_buf, std::size_t buff_len);
	static bool read_str_from_cfg(std::string file_name, std::string type, std::string key, std::string &out_str);
	static bool read_int_from_cfg(std::string file_name, std::string type, std::string key, int& out_data);

private:
	void _parse_cfg_line(std::string &line);
};