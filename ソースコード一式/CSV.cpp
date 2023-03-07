#include "CSV.h"

using namespace std;


// cell �̌^�ɉ������L���X�g�����̒�`
float CSVFile::CastCell(string _str) {
    return stof(_str);
}

// CSV�t�@�C����ǂ݂���
//template <typename T>
void CSVFile::CsvReadNum(string _filepath, bool _isHeader, bool _isIndex, char _delim) {
    this->m_filepath = _filepath;
    this->m_isHeader = _isHeader;
    this->m_isIndex = _isIndex;
    this->m_delim = _delim;

    string str_buf;
    string str_comma_buf;

    // �ǂݍ���csv�t�@�C�����J��(ifstream�̃R���X�g���N�^�ŊJ��)
    ifstream ifs_csv_file(_filepath);

    // getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e�� str_buf �Ɋi�[)
    for (int i = 0; getline(ifs_csv_file, str_buf); i++) {
        m_cell.push_back(vector<float>());

        // delim ��؂育�ƂɃf�[�^��ǂݍ��ނ��߂� istringstream �ɂ���
        istringstream i_stream(str_buf);

        for (int j = 0; getline(i_stream, str_comma_buf, _delim); j++) {
            if (_isHeader && _isIndex) { //�����^�Ȃ�w�b�_�[�A�C���f�b�N�X��ǂݍ���
                if (i == 0 && j == 0) continue;//0��ڂ��X�L�b�v
                if (i == 0 && j != 0) m_header.push_back(str_comma_buf);
                if (i != 0 && j == 0) m_index.push_back(str_comma_buf);
                //if (i != 0 && j != 0) m_cell.at(i - 1).push_back(cast_cell(str_comma_buf));
                if (i != 0 && j != 0)m_cell.at(i - 1).push_back(CastCell(str_comma_buf));   
                  
            }
            else if (_isHeader) {//�w�b�_�[���^�Ȃ�
                if (i == 0) m_header.push_back(str_comma_buf);
                if (i != 0 && j == 0) m_index.push_back(string());
                if (i != 0) m_cell.at(i - 1).push_back(CastCell(str_comma_buf));
            }
            else if (_isIndex) {//�C���f�b�N�X���^�Ȃ�
                if (i == 0 && j != 0) m_header.push_back(string());
                if (j == 0) m_index.push_back(str_comma_buf);
                if (j != 0) m_cell.at(i).push_back(CastCell(str_comma_buf));
            }
            else {//�����U�Ȃ�
                if (i == 0) m_header.push_back(string());
                if (j == 0) m_index.push_back(string());
                m_cell.at(i).push_back(CastCell(str_comma_buf));
            }
        }
    }
}

void CSVFile::CsvRead(std::string _filepath, char _delim)
{
    this->m_filepath = _filepath;
    this->m_isHeader = true;
    this->m_isIndex = true;
    this->m_delim = _delim;

    string str_buf;
    string str_comma_buf;

    // �ǂݍ���csv�t�@�C�����J��(ifstream�̃R���X�g���N�^�ŊJ��)
    ifstream ifs_csv_file(_filepath);

    // getline�֐���1�s���ǂݍ���(�ǂݍ��񂾓��e�� str_buf �Ɋi�[)
    for (int i = 0; getline(ifs_csv_file, str_buf); i++) {
        m_cell.push_back(vector<float>());

        // delim ��؂育�ƂɃf�[�^��ǂݍ��ނ��߂� istringstream �ɂ���
        istringstream i_stream(str_buf);
        
        for (int j = 0; getline(i_stream, str_comma_buf, _delim); j++) {
            if (m_isHeader && m_isIndex) { //�����^�Ȃ�w�b�_�[�A�C���f�b�N�X��ǂݍ���
                
                if (i == 0 && j != 0) m_header.push_back(str_comma_buf);
                else if (i != 0 && j == 0) m_index.push_back(str_comma_buf);
                else if (i != 0 && j == 1) m_modelName.push_back(str_comma_buf);//���f���t�@�C������ǂݍ���
                else if (i != 0 && j == 2) m_texfolder.push_back(str_comma_buf);//�e�N�X�`���t�@�C������ǂݍ���
                //if (i != 0 && j != 0) m_cell.at(i - 1).push_back(cast_cell(str_comma_buf));
                else if (i != 0 && j != 0)m_cell.at(i - 1).push_back(CastCell(str_comma_buf));
                else if (i == 0 && j == 0) continue;//0(����)���X�L�b�v
            }
            else if (m_isHeader) {//�w�b�_�[���^�Ȃ�
                if (i == 0) m_header.push_back(str_comma_buf);
                if (i != 0 && j == 0) m_index.push_back(string());
                if (i != 0) m_cell.at(i - 1).push_back(CastCell(str_comma_buf));
            }
            else if (m_isIndex) {//�C���f�b�N�X���^�Ȃ�
                if (i == 0 && j != 0) m_header.push_back(string());
                if (j == 0) m_index.push_back(str_comma_buf);
                if (j != 0) m_cell.at(i).push_back(CastCell(str_comma_buf));
            }
            else {//�����U�Ȃ�
                if (i == 0) m_header.push_back(string());
                if (j == 0) m_index.push_back(string());
                m_cell.at(i).push_back(CastCell(str_comma_buf));
            }
        }
    }
}

// CSV�t�@�C�����t�@�C���o�͂���
//template <typename T>
void CSVFile::CsvWriteOLD(string _filepath, char _delim) {
    // ��������csv�t�@�C�����J��(ofstream�̃R���X�g���N�^�ŊJ��)
    ofstream ofs_csv_file(_filepath);

    if (m_isHeader) {
        for (int j = 0; j < m_header.size(); j++) {
            if (m_isIndex && j == 0) ofs_csv_file << _delim;
            ofs_csv_file << m_header.at(j) << _delim;
        }
        ofs_csv_file << endl;
    }
    for (int i = 0; i < m_index.size(); i++) {
        if (m_isIndex) {
            ofs_csv_file << m_index.at(i) << _delim;
        }
        for (int j = 0; j < m_header.size(); j++) {
            ofs_csv_file << m_cell.at(i).at(j) << _delim;
        }
        ofs_csv_file << endl;
    }
    ofs_csv_file << endl;
}

// CSV�t�@�C�����t�@�C���o�͂���B�G�l�~�[�p
void CSVFile::CsvWrite(string _filepath, char _delim) {
    // ��������csv�t�@�C�����J��(ofstream�̃R���X�g���N�^�ŊJ��)
    ofstream ofs_csv_file(_filepath);

    if (m_isHeader) {
        for (int j = 0; j < m_header.size(); j++) {
            if (m_isIndex && j == 0) ofs_csv_file << _delim;
            ofs_csv_file << m_header.at(j) << _delim;
        }
        ofs_csv_file << endl;
    }
    for (int i = 0; i < m_index.size(); i++) {
        if (m_isIndex) {
            ofs_csv_file << m_index.at(i) << _delim;
            ofs_csv_file << m_modelName.at(i) << _delim;
            ofs_csv_file << m_texfolder.at(i) << _delim;
        }
        for (int j = 0; j < (m_header.size() - 2); j++) {
            ofs_csv_file << m_cell.at(i).at(j) << _delim;
        }
        ofs_csv_file << endl;
    }
    ofs_csv_file << endl;
}


// CSV�t�@�C�����R���\�[���o�͂���
//template <typename T>
void CSVFile::CsvShow() {
    cout << "filepath = " << m_filepath << ", ";
    cout << "isHeader = " << int(m_isHeader) << ", ";
    cout << "isIndex = " << int(m_isIndex) << ", ";
    cout << "delim = " << m_delim << endl;
    cout << "header size = " << m_header.size() << ", ";
    cout << "index size = " << m_index.size() << endl;

    if (m_isHeader) {
        for (int j = 0; j < m_header.size(); j++) {
            if (m_isIndex && j == 0) cout << "\t";
            //cout << m_header.at(j) << "(h)" << "\t";
            cout << m_header.at(j) << "\t";
        }
        cout << endl;
    }
    for (int i = 0; i < m_index.size(); i++) {
        if (m_isIndex) {
            //cout << m_index.at(i) << "(i)" << "\t";
            cout << m_index.at(i) << "\t";
        }
        for (int j = 0; j < m_header.size(); j++) {
            cout << m_cell.at(i).at(j) << "\t";
        }
        cout << endl;
    }
    cout << endl;
}