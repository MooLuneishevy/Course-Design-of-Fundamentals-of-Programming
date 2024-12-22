//�������ṹ��
struct admin{
    char admin_ID[100];//���񹤺�
    char password[100];//�����¼ϵͳ����
    char name[100];//��������
    struct admin*next;
};

//�����ʦ�ṹ��
struct teacher{
    char teacher_ID[100];//��ʦ����
    char password[100];//��ʦ��¼ϵͳ����
    char name[100];//��ʦ����
    char course[100];//��ʦ���ڿγ�
    char major[100];//��ʦ����רҵ
    int is_required_course;//�Ƿ���޿�
    double credit;//�γ�ѧ��
    struct teacher*next;
};

//����ѧ���ṹ��
struct student{
    char student_ID[100];//ѧ��ѧ��
    char password[100];//ѧ����¼ϵͳ����
    char name[100];//ѧ������
    char major[100];//ѧ������רҵ
    double GPA;//ѧ��GPA
    double GPA_add;//ѧ���ۼ����ʼӷ�
	double AvgScore;
    struct score_node*score_linked_list;//ѧ���ĳɼ�����
    struct student*next;
};

//����רҵ�ṹ�� ÿ��רҵ��Ӧһ��ѧ���������ʦ����
struct major{
    char name[100];//רҵ����
    struct student*student_linked_list;//רҵ��Ӧ��ѧ��
    struct teacher*teacher_linked_list;//רҵ��Ӧ�Ľ�ʦ
    int is_published;//רҵ���������Ƿ񷢲�
    int publish_time;//רҵ����������ʾʱ��
    struct major*next;
};

//����γ̽ṹ�� ÿ��ѧ����Ӧһ���γ�����
struct score_node{
    double score;//�γ̳ɼ�
    char subject_name[100];//�γ�����
    double credit;//ѧ��
    int is_required_course;//�Ƿ�Ϊ���޿�
    double grade_point;//���� ����score���м���
    struct score_node*next;
};

//����ʱ��ṹ��
struct time{
    int year;
    int month;
    int day;
};

//�������ļӷ�����
struct paper_node{
    char author[10][100];//�������߲�����ʮ����
    char paper_name[1000];//��������
    char journal_or_conference_name[100];//�ڿ����ƻ��������
    struct time publication_time;//���ķ���ʱ��
    int range;//����ڻ����ĺŻ�ҳ��
    char level[100];//���Ķ�Ӧ�ļ���
    double GPA_add;//���ʼӷ�
    struct paper_node*next;
};

//����󴴼ӷ�����
struct innovate_node{
    char member[5][100];//�����Ա�����������
    char instructor[100];//ָ����ʦ����
    char project_name[1000];//��Ŀ����
    char code[100];//��Ŀ���
    struct time initiation_time,completion_time;//����ʱ�������ʱ��
    double GPA_add;//���ʼӷ�
    struct innovate_node*next;
};

//���徺���ӷ�����
struct competition_node{
    char competition_name[1000];//��������
    char organizer[100];//���췽
    char awardee[10][100];//����񽱳�Ա������ʮ����
    char award_level[100];//�񽱵ȼ�
    struct time award_time;//��ʱ��
    double GPA_add;//���ʼӷ�
    struct competition_node*next;
};