//定义教务结构体
struct admin{
    char admin_ID[100];//教务工号
    char password[100];//教务登录系统密码
    char name[100];//教务姓名
    struct admin*next;
};

//定义教师结构体
struct teacher{
    char teacher_ID[100];//教师工号
    char password[100];//教师登录系统密码
    char name[100];//教师姓名
    char course[100];//教师教授课程
    char major[100];//教师所属专业
    int is_required_course;//是否必修课
    double credit;//课程学分
    struct teacher*next;
};

//定义学生结构体
struct student{
    char student_ID[100];//学生学号
    char password[100];//学生登录系统密码
    char name[100];//学生姓名
    char major[100];//学生所属专业
    double GPA;//学生GPA
    double GPA_add;//学生累计素质加分
	double AvgScore;
    struct score_node*score_linked_list;//学生的成绩链表
    struct student*next;
};

//定义专业结构体 每个专业对应一个学生链表与教师链表
struct major{
    char name[100];//专业名称
    struct student*student_linked_list;//专业对应的学生
    struct teacher*teacher_linked_list;//专业对应的教师
    int is_published;//专业推免排名是否发布
    int publish_time;//专业推免排名公示时长
    struct major*next;
};

//定义课程结构体 每个学生对应一个课程链表
struct score_node{
    double score;//课程成绩
    char subject_name[100];//课程名称
    double credit;//学分
    int is_required_course;//是否为必修课
    double grade_point;//绩点 根据score进行计算
    struct score_node*next;
};

//定义时间结构体
struct time{
    int year;
    int month;
    int day;
};

//定义论文加分链表
struct paper_node{
    char author[10][100];//假设作者不超过十个人
    char paper_name[1000];//论文名称
    char journal_or_conference_name[100];//期刊名称或会议名称
    struct time publication_time;//论文发布时间
    int range;//卷或期或论文号或页码
    char level[100];//论文对应的级别
    double GPA_add;//素质加分
    struct paper_node*next;
};

//定义大创加分链表
struct innovate_node{
    char member[5][100];//假设成员不超过五个人
    char instructor[100];//指导教师名称
    char project_name[1000];//项目名称
    char code[100];//项目编号
    struct time initiation_time,completion_time;//立项时间与结项时间
    double GPA_add;//素质加分
    struct innovate_node*next;
};

//定义竞赛加分链表
struct competition_node{
    char competition_name[1000];//竞赛名称
    char organizer[100];//主办方
    char awardee[10][100];//假设获奖成员不超过十个人
    char award_level[100];//获奖等级
    struct time award_time;//获奖时间
    double GPA_add;//素质加分
    struct competition_node*next;
};