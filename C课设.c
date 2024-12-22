#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<locale.h>
#include"struct.h"

//记录此时是否已经登录
int logged_in=0;
struct student*logged_in_stu;
struct teacher*logged_in_tea;
struct admin*logged_in_adm;

//链表初始化
struct major*major_head=NULL;
struct paper_node*paper_head=NULL;
struct innovate_node*innovate_head=NULL;
struct competition_node*competition_head=NULL;
struct admin*admin_head=NULL;
struct paper_node*paper_process_head=NULL;
struct innovate_node*innovate_process_head=NULL;
struct competition_node*competition_process_head=NULL;

//函数声明
//计算学生对应的GPA_add 
double cal_GPA_add(struct student*tmp);
//教务查看学术论文 
void admin_check_paper();
//教务查看大创项目 
void admin_check_innovate();
//教务查看竞赛获奖 
void admin_check_competition();
//教务查看素质加分项目 
void admin_check_GPA_add();
//学生修改个人信息 修改密码
void student_information_change(struct student* S);
//教师管理课程成绩
void teacher_course_manage(struct teacher* T);
//教师修改登陆密码
void teacher_information_change(struct teacher* T);
//教务修改登陆密码
void admin_information_change(struct admin* A);
//根据输入的课程成绩 计算对应的绩点
double find_gpa(double score);
//对于给定学生 计算其GPA
double calculate_GPA(struct student*stu);
//从文件读入专业信息
void read_major(FILE*fp);
//从文件中读入学生信息
void read_student(FILE*fp);
//从文件中读取教师信息
void read_teacher(FILE*fp);
//从文件中读取教务信息
void read_admin(FILE*fp);
//从文件中读入成绩信息
void read_score(FILE*fp);
//系统初始化函数 首先读入已有的学生 教师和教务信息
void system_initialization();
//程序主界面 选择学生 教师或者教务进行登录
void screen();
//学生登录账号密码验证
struct student*student_login_verify(char*major,char*ID,char*password);
//教师登录账号密码验证
struct teacher*teacher_login_verify(char*major,char*ID,char*password);
//教务登录账号密码验证
struct admin*admin_login_verify(char*ID,char*password);
//学生查询自己的GPA
void student_course_GPA_check(struct student* S);
//学生上报学术论文
void student_update_paper();
//学生上报大创项目
void student_update_innovate();
//学生上报竞赛获奖
void student_update_competition();
//学生上报学院素质加分情况 包括大创 论文和竞赛
void student_update_GPA_add();
//教师上传课程成绩
void teacher_course_upload();
//教务审核学术论文
void admin_verify_paper();
//教务审核大创项目
void admin_verify_innovate();
//教务审核竞赛获奖
void admin_verify_competition();
//教务审核素质加分情况
void admin_verify_GPA_add();
//教务转专业学籍异动管理 可以添加学生 或者删除学生
struct student* Find_change_stu(struct major* head ,char origin_major[100], char change_stu_ID[100]);
//转专业学生处理
void change_stu_major(struct major* head ,char origin_major[100], char new_major[100], char change_stu_ID[100]);
//教务进行转专业处理
void admin_major_change(struct major* head);
//论文链表插入操作
void InsertList();
//论文链表删除操作
void DeleteList(char paper[1000]);
//返回论文对应指针 不输出
struct paper_node* Find(struct paper_node* head, char paper[1000]);
//查看论文信息
void CheckList(struct paper_node* head, char paper[1000]);
//论文的更新操作
void UpdateList(struct paper_node* head, char paper[1000]);
//qsort比较函数 按照学业绩点降序排列
int compareGPA(const void *a, const void *b);
//qsort比较函数 按照综合绩点降序排列
int compareTotalGPA(const void *a, const void *b);
//qsort比较函数 按照平均成绩降序排列
int compareAvgScore(const void *a, const void *b);
//计算平均学业成绩 
double cal_AvgScore(struct student*tmp);
//平均学业成绩排名 
void rank_with_AvgScore(struct student* head, int num);
//qsort对某一门课成绩进行比较
int compareProgram(const void *a, const void *b);
//计算链表的长度
int getLength(struct student* head);
//综合绩点排名 qsort
void rank_with_GPA(struct student* head, int num,int choice);
//教务计算推免成绩并公布排名
void admin_calculate_GPA_added(struct major* major_head);
//选择专业并计算推免排名
//void rank_admin(struct major* major_head);
//学生查询推免排名情况 根据目前是否发布进行处理
void student_find_rank_M(struct major* major_head, char*name);
//学生查询 绩点排名 qsort
void student_find_rank(struct major* major_head, char*name);
//学生登录系统
void student_login();
//教师登录系统
void teacher_login();
//教务登录系统
void admin_login();

int day_of_month[12]={31,29,31,30,31,30,31,31,30,31,30,31};
//检验时间的有效性 
int check_time(struct time t){
	return (t.year>=2004 && t.year<=2024 && t.month>=1 && t.month<=12 && t.day>=1 && t.day<=day_of_month[t.month-1]);
}

//学生查询自己的 成绩 和 GPA
void student_course_GPA_check(struct student* S) {
	
	double gpa_academic = calculate_GPA(S);
	double avg_score = cal_AvgScore(S);
	
    printf("学业科目绩点 GPA: %.4lf\n", gpa_academic);
    double gpa_add=cal_GPA_add(S);
    printf("素质项目绩点 GPA: %.2lf\n", gpa_add);
    printf("综合成绩绩点 GPA: %.4lf\n", gpa_academic+gpa_add);
    printf("平均学业成绩    : %.4lf\n", avg_score);
    
    printf("\n"); 
	
	struct score_node* res = S->score_linked_list;
	if(res == NULL) 
	{
		printf("学生 %s 目前没有成绩信息!\n", S->name);
		printf("\n即将返回上层\n");
    	system("pause");
    	system("cls");
    	student_login();
    	return;
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("科目\t\t\t成绩\t\t学分\t\t绩点\t\t性质\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while(res != NULL)
	{
		char model[100];
		if(res->is_required_course == 1)
			strcpy(model, "必修");
		else
			strcpy(model, "选修");
		printf("%s\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%s\n", res->subject_name, res->score, res->credit, res->grade_point, model);
		res = res->next;
	}
    
    printf("\n即将返回上层\n");
    system("pause");
    system("cls");
    student_login();
    return;
}

//教师管理课程成绩
void teacher_course_manage(struct teacher* T) {
	system("cls");
    int choice;
    char choice_c[100];
    printf("欢迎登录教师课程管理系统 请选择功能\n");
    printf("***************************************\n");
    printf("* [1] 修改学生成绩  [2] 删除学生成绩  *\n");
    printf("* [3] 返回上层选择  [*] ************  *\n");
    printf("***************************************\n");
    gets(choice_c);
    choice=atoi(choice_c);
    if (choice == 1) {
    	system("cls");
        struct major* p = major_head;
        //找到教师对应的专业 
        while (p != NULL) {
            if (strcmp(p->name, T->major)==0)
                break;
            p = p->next;
        }
        struct student* q = p->student_linked_list;
        char xiugai[100];
        while(1){ 
	        printf("输入要修改成绩的学生学号:\n");
	        gets(xiugai);
	        if(strlen(xiugai)!=8)
	        	printf("输入学号长度错误 请重新输入\n");
	        else
	        	break;
    	}
        while (q != NULL) {
            if (strcmp(q->student_ID, xiugai) == 0)
                break;
            q = q->next;
        }
        if (q == NULL) {
            printf("学号错误 请重试\n");
            system("pause");
            teacher_course_manage(T);
            return;
        }
        double score = 0;
		char score_c[100];
        while(1){
	        printf("请输入课程的成绩:\n");
	        gets(score_c);
	        score=atof(score_c);
	        if(score>0 && score<=100)
	        	break;
	        else{
	        	printf("输入成绩有误 请重新输入\n");
			}
    	}
        struct score_node* SC = q->score_linked_list;
        while (SC != NULL) {
            if (strcmp(SC->subject_name, T->course) == 0) {
                SC->score = score;
                break;
            }
            SC = SC->next;
        }
        if(SC==NULL){
        	printf("课程成绩暂未上报 即将返回上级\n");
		}
		else{
        printf("修改成功 即将返回上级\n");
    	}
        system("pause");
        system("cls");
        teacher_login();
        return;
    }
    else if (choice == 2) {
    	system("cls");
        struct major* p = major_head;
        while (p != NULL) {
            if (strcmp(p->name, T->major)==0)
                break;
            p = p->next;
        }
        struct student* q = p->student_linked_list;
        printf("输入要删除成绩的学生学号:\n");
        char shanchu[100];
        while(1){
	        gets(shanchu);
	        if(strlen(shanchu)!=8)
	        	printf("学号输入有误 请重新输入\n");
	        else
	        	break;
    	}
        while (q != NULL) {
            if (strcmp(q->student_ID, shanchu) == 0)
                break;
            q = q->next;
        }
        if (q == NULL) {
            printf("学号错误 请重试\n");
            system("pause");
			teacher_course_manage(T);
            return;
        }
        struct score_node* SC = q->score_linked_list;
        while (SC->next != NULL) {
            if (strcmp(SC->next->subject_name, T->course) == 0) {
                struct score_node* TMP = SC->next;
                SC->next = TMP->next;
                free(TMP);
                break;
            }
            SC = SC->next;
        }
        if(SC->next==NULL){
        	printf("不存在此课程 即将返回上级\n");
		}
		else{
        	printf("删除成功 即将返回上级\n");
		}
        system("pause");
        system("cls");
        teacher_login();
		return; 
    }
    else if(choice==3){
    	teacher_login();
	}
    else{
    	printf("请输入正确的选项\n");
		system("pause");
		system("cls");
		teacher_course_manage(T);
		return;
	}
}

//学生修改个人信息 修改密码
void student_information_change(struct student*S){
	system("cls");
	printf("请输入更新后的密码\n"); 
    char change_password[20];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("密码不能为空 请重新输入\n");
        system("pause"); 
        student_information_change(S);
    }
    else{
	    printf("密码修改成功 即将返回上级\n");
	    strcpy(S->password,change_password);
	    system("pause");
	    student_login();
	}
    return;
}

//教师修改登陆密码
void teacher_information_change(struct teacher*T){
	system("cls");
	printf("请输入更新后的密码\n"); 
    char change_password[100];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("密码不能为空 请重新输入\n");
        system("pause"); 
        teacher_information_change(T);
    }
    else{
	    printf("密码修改成功 即将返回上级\n");
	    strcpy(T->password,change_password);
	    system("pause");
	    teacher_login();
	}
    return;
}

//教务修改登陆密码
void admin_information_change(struct admin*A){
	system("cls");
	printf("请输入更新后的密码\n"); 
    char change_password[100];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("密码不能为空 请重新输入\n");
        system("pause"); 
        admin_information_change(A);
    }
    else{
	    printf("密码修改成功 即将返回上级\n");
	    strcpy(A->password,change_password);
	    system("pause");
	    admin_login();
	}
    return;
}

//根据输入的课程成绩 计算对应的绩点
double find_gpa(double score){
    if(score>=90)
        return 4.0;
    else if(score>=87)
        return 3.7;
    else if(score>=84)
        return 3.3;
    else if(score>=80)
        return 3.0;
    else if(score>=77)
        return 2.7;
    else if(score>=74)
        return 2.3;
    else if(score>=70)
        return 2.0;
    else if(score>=67)
        return 1.7;
    else if(score>=64)
        return 1.3;
    else if(score>=60)
        return 1.0;
    else
        return 0;
}

//对于给定学生 计算其GPA
double calculate_GPA(struct student*stu){
    double sum_of_credit=0;
    double sum_of_grade_point=0;
    struct score_node*head=stu->score_linked_list;
    if(head==NULL){
        return 0;
    }
    while(head!=NULL){
        //选取必修课计算其对应绩点
        if(head->is_required_course==1){
            sum_of_credit+=head->credit;
            sum_of_grade_point+=head->grade_point*head->credit;
        }
        head=head->next;
    }
    if(sum_of_credit==0)
    	return 0;
    else
    	return sum_of_grade_point/sum_of_credit;
}

// 从文件读入专业信息
void read_major(FILE*fp){
    if(fp==NULL){
        printf("文件打开失败\n");
        system("pause");
        return;
    }
    int cnt=0;
    struct major*r;
    struct major*p=major_head;
    char tmp_name[100];
    while(fgets(tmp_name,sizeof(tmp_name),fp)){
    	cnt++;
        tmp_name[strcspn(tmp_name,"\n")]='\0';
        r=(struct major*)malloc(sizeof(struct major));
        strncpy(r->name,tmp_name,sizeof(r->name));
        r->student_linked_list=NULL;
        r->teacher_linked_list=NULL;
        r->is_published=0;
        r->publish_time=0;
        if(major_head==NULL){
            major_head=r;
            p=r;
        }
        else{
            p->next=r;
            p=p->next;
        }
        p->next=NULL;
    }
    printf("总计读入 %d 个专业信息\n",cnt); 
    fclose(fp);
    return;
}

//从文件中读入学生信息
void read_student(FILE*fp){
    if(fp==NULL){
        printf("文件打开失败\n");
        system("pause");
        return;
    }
    int cnt=0;
    struct major*q=major_head;
    struct student*r,*p;
    char tmp_ID[100];
    char tmp_password[100];
    char tmp_name[100];
    char tmp_major[100];
    //依次输入 学工号 密码 姓名 专业
    while(fgets(tmp_ID,sizeof(tmp_ID),fp)){
        tmp_ID[strcspn(tmp_ID,"\n")]='\0';
        fgets(tmp_password,sizeof(tmp_password),fp);
        tmp_password[strcspn(tmp_password,"\n")]='\0';
        fgets(tmp_name,sizeof(tmp_name),fp);
        tmp_name[strcspn(tmp_name,"\n")]='\0';
        fgets(tmp_major,sizeof(tmp_major),fp);
        tmp_major[strcspn(tmp_major,"\n")]='\0';
        q=major_head;
        cnt++;
        while(q!=NULL){
            if(strcmp(q->name,tmp_major)==0)
                break;
            q=q->next;
        }
        if(q==NULL){
        	printf("在读取第 %d 位学生时 发生错误\n",cnt); 
            printf("不存在此专业: %s 读取错误\n",tmp_major);
            continue;
        }
        //此时q指向学生对应的专业 将学生插入到学生链表的末尾
        p=q->student_linked_list;
        if(p!=NULL){
            while(p->next!=NULL)
                p=p->next;
        }
        //依次保存学生的 学工号 密码 姓名 专业
        r=(struct student*)malloc(sizeof(struct student));
        strcpy(r->student_ID,tmp_ID);
        strcpy(r->password,tmp_password);
        strcpy(r->name,tmp_name);
        strcpy(r->major,tmp_major);
        r->score_linked_list=NULL;
        r->GPA_add=0; 
        if(q->student_linked_list==NULL){
            q->student_linked_list=r;
            p=r;
        }
        else{
            p->next=r;
            p=p->next;
        }
        p->next=NULL;
    }
    printf("总计读入 %d 位学生信息\n",cnt);
    fclose(fp);
    return;
}

//从文件中读取教师信息
void read_teacher(FILE*fp){
    if(fp==NULL){
        printf("文件打开失败\n");
        system("pause");
        return;
    }
    int cnt=0;
    char tmp_ID[100];
    char tmp_password[100];
    char tmp_name[100];
    char tmp_course[100];
    char tmp_major[100];
    char tmp_is_required_course_c[100];
    char tmp_credit_c[100];
    int tmp_is_required_course;
    double tmp_credit;
    struct major*q;
    struct teacher*r,*p;
    //依次读入 学工号 密码 姓名 讲授课程 专业 是否必修课 课程学分
    while(fgets(tmp_ID,sizeof(tmp_ID),fp)){
    	cnt++;
        tmp_ID[strcspn(tmp_ID,"\n")]='\0';
        fgets(tmp_password,sizeof(tmp_password),fp);
        tmp_password[strcspn(tmp_password,"\n")]='\0';
        fgets(tmp_name,sizeof(tmp_name),fp);
        tmp_name[strcspn(tmp_name,"\n")]='\0';
        fgets(tmp_course,sizeof(tmp_course),fp);
        tmp_course[strcspn(tmp_course,"\n")]='\0';
        fgets(tmp_major,sizeof(tmp_major),fp);
        tmp_major[strcspn(tmp_major,"\n")]='\0';
        fgets(tmp_is_required_course_c,sizeof(tmp_is_required_course_c),fp);
        tmp_is_required_course_c[strcspn(tmp_is_required_course_c,"\n")]='\0';
        fgets(tmp_credit_c,sizeof(tmp_credit_c),fp);
        tmp_credit_c[strcspn(tmp_credit_c,"\n")]='\0';
        q=major_head;
        while(q!=NULL){
            if(strcmp(q->name,tmp_major)==0)
                break;
            q=q->next;
        }
        if(q==NULL){
        	printf("在读入第 %d 位教师时 发生错误\n",cnt);
            printf("不存在此专业: %s 读取错误\n",tmp_major);
            continue;
        }
        //此时q指向教师对应的专业节点 将教师插入到教师链表的末尾
        p=q->teacher_linked_list;
        if(p!=NULL){
            while(p->next!=NULL)
                p=p->next;
        }
        //依次保存教师的 学工号 密码 姓名 讲授课程 专业 是否为必修课 课程学分
        r=(struct teacher*)malloc(sizeof(struct teacher));
        strcpy(r->teacher_ID,tmp_ID);
        strcpy(r->password,tmp_password);
        strcpy(r->name,tmp_name);
        strcpy(r->course,tmp_course);
        strcpy(r->major,tmp_major);
        tmp_is_required_course=atoi(tmp_is_required_course_c);
        r->is_required_course=tmp_is_required_course;
        tmp_credit=atof(tmp_credit_c);
        r->credit=tmp_credit;
        if(q->teacher_linked_list==NULL){
            q->teacher_linked_list=r;
            p=r;
        }
        else{
            p->next=r;
            p=p->next;
        }
        p->next=NULL;
    }
    printf("总计读入 %d 位教师信息\n",cnt); 
    fclose(fp);
    return;
}

//从文件中读取教务信息
void read_admin(FILE*fp){
    if(fp==NULL){
        printf("文件打开失败\n");
        system("pause");
        return;
    }
    int cnt=0;
    struct admin*r;
    struct admin*p=admin_head;
    char tmp_ID[100];
    char tmp_password[100];
    char tmp_name[100];
    //依次读入 ID 密码 姓名
    while(fgets(tmp_ID,sizeof(tmp_ID),fp)){
    	cnt++;
        tmp_ID[strcspn(tmp_ID,"\n")]='\0';
        fgets(tmp_password,sizeof(tmp_password),fp);
        tmp_password[strcspn(tmp_password,"\n")]='\0';
        fgets(tmp_name,sizeof(tmp_name),fp);
        tmp_name[strcspn(tmp_name,"\n")]='\0';
        r=(struct admin*)malloc(sizeof(struct admin));
        strcpy(r->admin_ID,tmp_ID);
        strcpy(r->password,tmp_password);
        strcpy(r->name,tmp_name);
        if(admin_head==NULL){
            admin_head=r;
            p=r;
        }
        else{
            p->next=r;
            p=p->next;
        }
        p->next=NULL;
    }
    printf("总计读入 %d 位教务信息\n",cnt);
    fclose(fp);
    return;
}

//从文件中读入成绩信息
void read_score(FILE*fp){
    if(fp==NULL){
        printf("文件打开失败\n");
        system("pause");
        return;
    }
    int cnt_stu=0;
    int cnt_cour=0;
    struct major*q;
    struct student*o;
    struct score_node*r,*p;
    char tmp_ID[100];
    char tmp_major[100];
    //该学生对应的课程数目
    char tmp_course_count_c[100];
    int tmp_course_count;
    //成绩节点对应的属性
    char tmp_score_c[100];
    double tmp_score;
    char tmp_subject_name[100];
    char tmp_credit_c[100];
    double tmp_credit;
    char tmp_is_required_course_c[100];
    int tmp_is_required_course;
    //依次读入 学生ID 学生专业 课程数目
    while(fgets(tmp_ID,sizeof(tmp_ID),fp)){
    	cnt_stu++;
        tmp_ID[strcspn(tmp_ID,"\n")]='\0';
        fgets(tmp_major,sizeof(tmp_major),fp);
        tmp_major[strcspn(tmp_major,"\n")]='\0';
        fgets(tmp_course_count_c,sizeof(tmp_course_count_c),fp);
        tmp_course_count_c[strcspn(tmp_course_count_c,"\n")]='\0';
        //首先 根据tmp_major寻找专业
        q=major_head;
        while(q!=NULL){
            if(strcmp(q->name,tmp_major)==0)
                break;
            q=q->next;
        }
        if(q==NULL){
        	printf("在读入第 %d 位学生成绩时 出现错误\n",cnt_stu);
            printf("不存在此专业: %s 读取错误\n",tmp_major);
            continue;
        }
        //随后 根据tmp_ID寻找对应的学生
        o=q->student_linked_list;
        while(o!=NULL){
            if(strcmp(o->student_ID,tmp_ID)==0)
                break;
            o=o->next;
        }
        //最后 读入该学生对应的成绩信息
        p=o->score_linked_list;
        //依次读入 成绩 课程名称 学分 是否为必修课
        tmp_course_count=atoi(tmp_course_count_c);
        while(tmp_course_count>0){
        	cnt_cour++;
            fgets(tmp_score_c,sizeof(tmp_score_c),fp);
            tmp_score_c[strcspn(tmp_score_c,"\n")]='\0';
            fgets(tmp_subject_name,sizeof(tmp_subject_name),fp);
            tmp_subject_name[strcspn(tmp_subject_name,"\n")]='\0';
            fgets(tmp_credit_c,sizeof(tmp_credit_c),fp);
            tmp_credit_c[strcspn(tmp_credit_c,"\n")]='\0';
            fgets(tmp_is_required_course_c,sizeof(tmp_is_required_course_c),fp);
            tmp_is_required_course_c[strcspn(tmp_is_required_course_c,"\n")]='\0';
            r=(struct score_node*)malloc(sizeof(struct score_node));
            tmp_score=atof(tmp_score_c);
            r->score=tmp_score;
            strcpy(r->subject_name,tmp_subject_name);
            tmp_credit=atof(tmp_credit_c);
            r->credit=tmp_credit;
            tmp_is_required_course=atoi(tmp_is_required_course_c);
            r->is_required_course=tmp_is_required_course;
            r->grade_point=find_gpa(r->score);
            if(o->score_linked_list==NULL){
                o->score_linked_list=r;
                p=r;
            }
            else{
                p->next=r;
                p=p->next;
            }
            p->next=NULL;
            tmp_course_count--;
        }
        //读入下一个学生之前 计算该学生当前的GPA情况
        o->GPA=calculate_GPA(o);
    }
    printf("总计读入 %d 位学生的成绩数据\n",cnt_stu);
    printf("总计读入 %d 门课程的成绩数据\n",cnt_cour);
    fclose(fp);
    return;
}

//系统初始化函数 首先读入已有的学生 教师和教务信息
void system_initialization(){
    //读入专业信息
    FILE*fp_major=fopen("major_data.txt","r");
    printf("正在初始化专业信息 请稍等\n");
    read_major(fp_major);
    printf("-----------------------------------------------------\n");
    //读入学生信息
    FILE*fp_student=fopen("student_data.txt","r");
    printf("正在初始化学生信息 请稍等\n");
    read_student(fp_student);
    printf("-----------------------------------------------------\n");
    //读入教师信息
    FILE*fp_teacher=fopen("teacher_data.txt","r");
    printf("正在初始化教师信息 请稍等\n");
    read_teacher(fp_teacher);
    printf("-----------------------------------------------------\n");
    //读入教务信息
    FILE*fp_admin=fopen("admin_data.txt","r");
    printf("正在初始化教务信息 请稍等\n");
    read_admin(fp_admin);
    printf("-----------------------------------------------------\n");
    //读入成绩信息
    FILE*fp_score=fopen("score_data.txt","r");
    printf("正在初始化成绩信息 请稍等\n");
    read_score(fp_score);
    printf("-----------------------------------------------------\n");
    printf("系统初始化成功 即将跳转吉林大学素质加分与成绩管理系统\n");
    printf("-----------------------------------------------------\n");
    system("pause"); 
    return;
}

//程序主界面 选择学生 教师或者教务进行登录
void screen(){
	system("cls");
    int choice=0;
	char choice_c[100];
    printf("欢迎登录吉林大学素质加分与成绩管理系统 请选择功能\n");
    printf("*******************************\n");
    printf("* [1] 学生登录  [2] 教师登录  *\n");
    printf("* [3] 教务登录  [4] 退出系统  *\n");
    printf("*******************************\n");
    printf("请选择:\n");
    gets(choice_c);
    choice=atoi(choice_c);
    if(choice==1)
        student_login();
    else if(choice==2)
        teacher_login();
    else if(choice==3)
        admin_login();
    else if(choice==4){
    	printf("系统成功登出 欢迎下次登录\n");
        exit(0);
    }
    else{
        printf("请输入正确的选择\n");
        system("pause");
        system("cls");
        screen();
    }
}

//学生登录账号密码验证
struct student*student_login_verify(char*major,char*ID,char*password){
	struct major*maj=major_head;
	while(maj!=NULL){
		if(strcmp(maj->name,major)==0)
			break;
		maj=maj->next;
	}
	if(maj==NULL){
		return NULL;
	}
	struct student*stu=maj->student_linked_list;
    while(stu!=NULL){
        if(strcmp(ID,stu->student_ID)==0 && strcmp(password,stu->password)==0)
            return stu;
        stu=stu->next; 
    }
    return NULL;
}

//教师登录账号密码验证
struct teacher*teacher_login_verify(char*major,char*ID,char*password){
	struct major*maj=major_head;
	while(maj!=NULL){
		if(strcmp(maj->name,major)==0)
			break;
		maj=maj->next;
	}
	if(maj==NULL){
		return NULL;
	}
    struct teacher*tea=maj->teacher_linked_list;
    while(tea!=NULL){
        if(strcmp(ID,tea->teacher_ID)==0 && strcmp(password,tea->password)==0)
            return tea;
        tea=tea->next;
    }
    return NULL;
}

//教务登录账号密码验证
struct admin*admin_login_verify(char*ID,char*password){
    struct admin*adm=admin_head;
    while(adm!=NULL){
        if(strcmp(ID,adm->admin_ID)==0 && strcmp(password,adm->password)==0)
            return adm;
        adm=adm->next; 
    }
    return NULL;
}

//学生上报学术论文
void student_update_paper(){
    struct paper_node*p=paper_process_head;
    //p指向paper_process_head的最后一个节点 
    if(p!=NULL){
		while(p->next!=NULL)
	        p=p->next;
    }
    struct paper_node*r=(struct paper_node*)malloc(sizeof(struct paper_node));
    printf("请输入作者数目与作者姓名:\n");
    printf("注意 本系统视最后一位作者为通讯作者\n");
    char cnt_c[100];
    int cnt=0;
    while(1){
	    gets(cnt_c);
	    cnt=atoi(cnt_c);
	    if(cnt<=0 || cnt>=10)
	    	printf("作者数目输入有误 请重新输入\n");
	    else
	    	break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){
	    	printf("请输入第%d作者:\n",i+1);
	        gets(r->author[i]);
	        if(strlen(r->author[i])==0)
	        	printf("作者不允许为空 请重新输入\n");
	        else 
	        	break;
    	}
    }
    for(int i=cnt;i<10;i++){
    	r->author[i][0]='\0';
	}
	while(1){
	    printf("请输入论文名称:\n");
	    gets(r->paper_name);
		if(strlen(r->paper_name)==0)
			printf("论文名称不允许为空 请重新输入\n");
		else
			break; 
	}
	while(1){
	    printf("请输入期刊名称或会议名称:\n");
	    gets(r->journal_or_conference_name);
	    if(strlen(r->journal_or_conference_name)==0)
	    	printf("期刊名称或会议名称不允许为空 请重新输入\n");
	    else
	    	break;
	}
	while(1){
	    printf("请输入论文发布时间 以年月日格式输入:\n");
	    char year_c[100];
	    gets(year_c);
	    r->publication_time.year=atoi(year_c);
	    char month_c[100];
	    gets(month_c);
	    r->publication_time.month=atoi(month_c);
	    char day_c[100];
	    gets(day_c);
	    r->publication_time.day=atoi(day_c);
	    if(check_time(r->publication_time))
	    	break;
	    else
	    	printf("论文发布时间输入错误 请重新输入\n");
	}
	char range_c[100];
	while(1){
	    printf("请输入卷或期或论文号或页码:\n");
	    gets(range_c);
	    r->range=atoi(range_c);
	    if(r->range>0)
	    	break;
	    else
	    	printf("卷或期或论文号或页码输入错误 请重新输入\n");
	}
	while(1){
	    printf("请输入论文级别\n");
	    gets(r->level);
	    if(strlen(r->level)==0)
	    	printf("论文级别不允许为空 请重新输入\n");
	    else
	    	break;
	}
	char GPA_add_c[100];
	while(1){
	    printf("请输入您预期的素质加分:\n");
	    gets(GPA_add_c);
	    r->GPA_add=atof(GPA_add_c);
	    if(r->GPA_add<=0 || r->GPA_add>0.4)
	    	printf("输入素质加分非法 请重新输入\n");
	    else
	    	break;
	}
    if(paper_process_head==NULL){
    	paper_process_head=r;
    	p=r;
	}
	else{
	    p->next=r;
	    p=p->next;
	}
	p->next=NULL;
    printf("上报成功 请耐心等待教务审核\n");
    printf("接下来将返回上报界面\n");
    system("pause");
    system("cls");
    student_update_GPA_add();
}

//学生上报大创项目
void student_update_innovate(){
    struct innovate_node*p=innovate_process_head;
    if(p!=NULL){
	    while(p->next!=NULL)
	        p=p->next;
    }
    struct innovate_node*r=(struct innovate_node*)malloc(sizeof(struct innovate_node));
    char cnt_c[100];
	int cnt=0;
	while(1){
	    printf("请输入成员人数与成员姓名:\n");
	    gets(cnt_c);
	    cnt=atoi(cnt_c);
	    if(cnt<=0 || cnt>5){
	    	printf("成员人数输入错误 请重新输入\n"); 
		}
		else
			break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){
	    	printf("请输入第%d位成员:\n",i+1);
	        gets(r->member[i]);
	        if(strlen(r->member[i])==0)
	        	printf("大创成员不允许为空 请重新输入\n");
	        else 
	        	break;
    	}
    }
    for(int i=cnt;i<5;i++){
    	r->member[i][0]='\0';
	}
	while(1){
	    printf("请输入指导教师姓名:\n");
	    gets(r->instructor);
	    if(strlen(r->instructor)==0)
	    	printf("指导教师不允许为空 请重新输入\n");
	    else
	    	break;
	}
	while(1){
	    printf("请输入项目名称:\n");
	    gets(r->project_name);
	    if(strlen(r->project_name)==0)
	    	printf("项目名称不允许为空 请重新输入\n");
	    else
	    	break;
	}
	while(1){
	    printf("请输入项目编号:\n");
		gets(r->code);
	    if(strlen(r->code)==0)
	    	printf("项目编号不允许为空 请重新输入\n");
	    else
	    	break;
	}
	char year_c[100];
	char month_c[100];
	char day_c[100];
	while(1){ 
		while(1){
		    printf("请输入大创立项时间 以年月日格式输入:\n");
		    gets(year_c);
		    r->initiation_time.year=atoi(year_c);
		    gets(month_c);
		    r->initiation_time.month=atoi(month_c);
		    gets(day_c);
		    r->initiation_time.day=atoi(day_c);
		    if(check_time(r->initiation_time)){
		    	break;
			}
			else{
				printf("输入时间无效 请重新输入\n");
			}
		}
		while(1){
		    printf("请输入大创结项时间 以年月日格式输入:\n");
		    gets(year_c);
		    r->completion_time.year=atoi(year_c);
		    gets(month_c);
		    r->completion_time.month=atoi(month_c);
		    gets(day_c);
		    r->completion_time.day=atoi(day_c);
		    if(check_time(r->completion_time)){
		    	break;
			}
			else{
				printf("输入时间无效 请重新输入\n");
			}
		}
		if(r->initiation_time.year>r->completion_time.year)
			printf("立项结项时间有误 请重新输入\n");
		else if(r->initiation_time.year==r->completion_time.year && r->initiation_time.month>r->completion_time.month)
			printf("立项结项时间有误 请重新输入\n");
		else if(r->initiation_time.year==r->completion_time.year && r->initiation_time.month==r->completion_time.month && r->initiation_time.day>r->completion_time.day)
			printf("立项结项时间有误 请重新输入\n");
		else
			break;
	}
    if(innovate_process_head==NULL){
    	innovate_process_head=r;
    	p=r;
	}
	else{
		p->next=r;
    	p=p->next;
	}
	p->next=NULL;
    printf("上报成功 请耐心等待教务审核\n");
    printf("接下来将返回上报界面\n");
    system("pause"); 
    system("cls");
    student_update_GPA_add();
}

//学生上报竞赛获奖
void student_update_competition(){
	//找到待审核的最后一个节点 
    struct competition_node*p=competition_process_head;
    if(p!=NULL){
		while(p->next!=NULL)
	        p=p->next;
    }
    struct competition_node*r=(struct competition_node*)malloc(sizeof(struct competition_node));
    char cnt_c[100];
    int cnt;
	while(1){ 
	    printf("请输入获奖人数与获奖成员姓名:\n");
		gets(cnt_c);
		cnt=atoi(cnt_c);
		if(cnt<=0 || cnt>10)
			printf("输入获奖人数非法 请重新输入\n");
		else
			break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){ 
	    	printf("请输入第%d位获奖人:\n",i+1);
	        gets(r->awardee[i]);
	        if(strlen(r->awardee[i])==0)
	        	printf("获奖人不允许为空 请重新输入\n");
	        else
	        	break;
    	}
    }
    for(int i=cnt;i<10;i++){
    	r->awardee[i][0]='\0';
	}
	while(1){
	    printf("请输入竞赛名称:\n");
	    gets(r->competition_name);
	    if(strlen(r->competition_name)==0)
	    	printf("竞赛名称不允许为空 请重新输入\n");
	    else
	    	break;
	}
	while(1){
	    printf("请输入竞赛主办方名称:\n");
	    gets(r->organizer);
	    if(strlen(r->organizer)==0)
	    	printf("竞赛主办方不允许为空 请重新输入\n");
		else
			break; 
	}
    char year_c[100];
	char month_c[100];
	char day_c[100];
    while(1){
		printf("请输入获奖时间 以年月日格式输入:\n");
	    gets(year_c);
	    r->award_time.year=atoi(year_c);
	    gets(month_c);
	    r->award_time.month=atoi(month_c);
	    gets(day_c);
	    r->award_time.day=atoi(day_c);
	    if(check_time(r->award_time)){
	    	break;
		} 
		else{
			printf("输入时间无效 请重新输入\n");
		}
	}
	while(1){
	    printf("请输入获奖等级:\n");
	    gets(r->award_level);
	    if(strlen(r->award_level)==0)
	    	printf("获奖等级不允许为空 请重新输入\n");
	    else
	    	break;
	}
	char GPA_add_c[100];
	while(1){ 
	    printf("请输入您预期的素质加分:\n");
	    gets(GPA_add_c);
	    r->GPA_add=atof(GPA_add_c);
	    if(r->GPA_add>0.4 || r->GPA_add<=0)
			printf("素质加分非法 请重新输入\n");
		else
			break; 
	}
    if(competition_process_head==NULL){
    	competition_process_head=r;
    	p=r;
	}
	else{
	    p->next=r;
	    p=p->next;
	}
	p->next=NULL;
    printf("上报成功 请耐心等待教务审核\n");
    printf("接下来将返回上报界面\n");
    system("pause");
    system("cls");
    student_update_GPA_add();
}

//学生上报学院素质加分情况 包括大创 论文和竞赛
void student_update_GPA_add(){
    int choice;
    char choice_c[100];
    system("cls");
    printf("请选择上报项目\n");
    printf("**************************************\n");
    printf("* [1] 上报学术论文  [2] 上报大创项目 *\n");
    printf("* [3] 上报竞赛获奖  [4] 返回上级选择 *\n");
    printf("**************************************\n");
    gets(choice_c);
    choice=atoi(choice_c);
    if(choice==1){
        system("cls");
        student_update_paper();
    }
    else if(choice==2){
        system("cls");
        student_update_innovate();
    }
    else if(choice==3){
        system("cls");
        student_update_competition();
    }
    else if(choice==4){
        system("cls");
        student_login();
    }
    else{
        printf("请输入正确的选择\n");
        system("pause");
        system("cls");
        student_update_GPA_add();
    }
    return;
}

//教师上传课程成绩
void teacher_course_upload(struct teacher*tea){
    struct major*q=major_head;
    while(q!=NULL){
        if(strcmp(q->name,tea->major)==0)
            break;
        q=q->next;
    }
    //此时无需判断是否为空 因为必定存在此专业 
    struct student*o=q->student_linked_list;
    if(o==NULL){
    	printf("此专业学生为空 上传失败\n");
	    printf("即将返回上层\n");
	    system("pause");
	    system("cls");
	    teacher_login();
	    return;
	}
	printf("即将输入学生成绩:\n");
	printf("课程名称:%s\n",tea->course); 
    printf("课程学分:%2lf\n",tea->credit);
    printf("课程属性:");
    if(tea->is_required_course){
    	printf("必修课\n");
	}
	else{
		printf("选修课\n");
	}
	char score_c[100];
    while(o!=NULL){
        struct score_node*r;
		struct score_node*p=o->score_linked_list;
        if(p!=NULL){
	        while(p->next!=NULL)
	            p=p->next;
        }
        r=(struct score_node*)malloc(sizeof(struct score_node));
        while(1){
	        printf("请输入学生%s的成绩:\n",o->name);
	        gets(score_c);
	        r->score=atof(score_c);
	        if(r->score<=0 || r->score>100)
	        	printf("成绩输入有误 请重新输入\n");
	        else
	        	break;
    	}
        r->grade_point=find_gpa(r->score);
        strcpy(r->subject_name,tea->course);
        r->credit=tea->credit;
        r->is_required_course=tea->is_required_course;
        if(o->score_linked_list=NULL){
        	o->score_linked_list=r;
        	p=r;
		}
		else{
	        p->next=r;
	        r->next=NULL;
    	}
    	p->next=NULL;
        o=o->next;
    }
    printf("上传成功\n");
    printf("即将返回上层界面\n");
    system("pause");
    system("cls");
    teacher_login();
    return;
}

//教务审核学术论文
void admin_verify_paper(){
    struct paper_node*p=paper_process_head;
    printf("欢迎登录学术论文审核系统\n");
    if(p==NULL){
        printf("暂无需要审核的学术论文\n");
        printf("即将返回上级\n");
        system("pause");
        system("cls");
        admin_verify_GPA_add();
        return;
    }
    else{
    	struct paper_node*test=p;
    	int cnt=0;
    	while(test!=NULL){
    		test=test->next;
    		cnt++;
		}
		printf("此时未审核的学术论文有 %d 篇\n",cnt);
		printf("即将开始审核\n"); 
	}
    //审核持续到链表的末尾
    while(p!=NULL){
        printf("论文作者为:\n");
        for(int i=0;i<10;i++){
        	if(p->author[i][0]=='\0')
        		break;
        	printf("成员%d. %s\n",i+1,p->author[i]);
		}
        printf("论文名称为:%s\n",p->paper_name);
        printf("期刊名称或会议名称为:%s\n",p->journal_or_conference_name);
        printf("论文发布时间为:%d年%d月%d日\n",p->publication_time.year,p->publication_time.month,p->publication_time.day);
        printf("卷或期或论文号或页码为:%d\n",p->range);
        printf("论文等级为:%s\n",p->level);
        printf("预期的素质加分为:%.2lf\n",p->GPA_add);
        int choice;
        char choice_c[100];
        printf("请输入审核结果:\n");
	    printf("********************************\n");
	    printf("* [1] 审核通过  [2] 审核不通过 *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        //如果审核通过 插入到论文链表
        if(choice==1){
            //如果论文链表为空
            if(paper_head==NULL){
                paper_head=p;
                p=p->next;
                paper_process_head=paper_process_head->next;
                paper_head->next=NULL;
            }
            //论文链表非空 插入到尾节点 
            else{
                struct paper_node*q=paper_head;
                while(q->next!=NULL)
                    q=q->next;
				q->next=p;
                q=q->next;
                p=p->next;
				paper_process_head=paper_process_head->next;
                q->next=NULL;
            }
        }
        else if(choice==2){
        	paper_process_head=paper_process_head->next;
			p=paper_process_head;
        }
        else{
            printf("输入有误 请重新输入\n");
            system("pause");
            system("cls");
        }
    }
    printf("审核成功 即将返回上级\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//教务审核大创项目
void admin_verify_innovate(){
    struct innovate_node*p=innovate_process_head;
    printf("欢迎登录大创项目审核系统\n");
    if(p==NULL){
        printf("暂无需要审核的大创项目\n");
        printf("即将返回上级\n");
        system("pause");
        system("cls");
		admin_verify_GPA_add();
        return;
    }
    else{
    	struct innovate_node*test=p;
    	int cnt=0;
    	while(test!=NULL){
    		test=test->next;
    		cnt++;
		}
		printf("此时未审核的大创项目有 %d 项\n",cnt);
		printf("即将开始审核\n");
	} 
    //审核持续到链表的末尾
    while(p!=NULL){
        printf("大创项目名称为:%s\n",p->project_name);
        printf("指导教师名称为:%s\n",p->instructor);
        printf("项目编号为:%s\n",p->code);
        printf("大创立项时间为:%d年%d月%d日\n",p->initiation_time.year,p->initiation_time.month,p->initiation_time.day);
        printf("大创结项时间为:%d年%d月%d日\n",p->completion_time.year,p->completion_time.month,p->completion_time.day);
        printf("大创成员为:\n");
        for(int i=0;i<5;i++){
        	if(p->member[i][0]=='\0')
        		break;
        	printf("成员%d. %s\n",i+1,p->member[i]);
		}
        int choice;
        char choice_c[100]; 
        printf("请输入审核结果\n");
	    printf("********************************\n");
	    printf("* [1] 审核通过  [2] 审核不通过 *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
        	p->GPA_add=0.1;
            //如果大创链表为空
            if(innovate_head==NULL){
                innovate_head=p;
				p=p->next;
                innovate_process_head=innovate_process_head->next;
                innovate_head->next=NULL;
            }
            //大创链表非空
            else{
                struct innovate_node*q=innovate_head;
                while(q->next!=NULL)
                    q=q->next;
                q->next=p;
                q=q->next;
                p=p->next;
                innovate_process_head=innovate_process_head->next;
                q->next=NULL;
            }
        }
        else if(choice==2){
        	innovate_process_head=innovate_process_head->next;
			p=innovate_process_head;
        }
        else{
            printf("输入有误 请重新输入\n");
        	system("pause");
            system("cls");
        }
    }
    printf("审核成功 即将返回上级\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//教务审核竞赛获奖
void admin_verify_competition(){
    struct competition_node*p=competition_process_head;
    printf("欢迎登录竞赛获奖审核系统\n");
    if(p==NULL){
        printf("暂无需要审核的竞赛获奖\n");
        printf("即将返回上级\n");
    	system("pause");
        system("cls");
		admin_verify_GPA_add();
        return;
    }
    else{
    	struct competition_node*test=p;
    	int cnt=0;
    	while(test!=NULL){
    		test=test->next;
    		cnt++;
		}
		printf("此时未审核的竞赛获奖有 %d 项\n",cnt);
		printf("即将开始审核\n");
	}
    //审核持续到链表的末尾
    while(p!=NULL){
        printf("竞赛名称为:%s\n",p->competition_name);
        printf("竞赛主办方为:%s\n",p->organizer);
        printf("获奖成员为:%s\n",p->awardee);
        for(int i=0;i<10;i++){
        	if(p->awardee[i][0]=='\0')
        		break;
        	printf("成员%d. %s\n",i+1,p->awardee[i]);
		}
        printf("获奖时间为:%d年%d月%d日\n",p->award_time.year,p->award_time.month,p->award_time.day);
        printf("获奖等级为:\n");
        printf("预期的素质加分为:%.2lf\n",p->GPA_add);
        int choice;
        char choice_c[100]; 
        printf("请输入审核结果\n");
	    printf("********************************\n");
	    printf("* [1] 审核通过  [2] 审核不通过 *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
            //如果竞赛链表为空
            if(competition_head==NULL){
                competition_head=p;
                p=p->next;
                competition_process_head=competition_process_head->next;
            	competition_head->next=NULL;
            }
            //竞赛链表非空
            else{
                struct competition_node*q=competition_head;
                while(q->next!=NULL)
                    q=q->next;
                q->next=p;
                q=q->next;
                p=p->next;
                competition_process_head=competition_process_head->next;
                q->next=NULL;
            }
        }
        else if(choice==2){
        	competition_process_head=competition_process_head->next;
        	p=competition_process_head;
		}
        else{
            printf("输入有误 请重新输入\n");
            system("pause");
            system("cls");
        }
    }
    printf("审核成功 即将返回上级\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//教务审核素质加分情况
void admin_verify_GPA_add(){
    int choice;
    char choice_c[100]; 
    system("cls");
    printf("欢迎登入素质加分审核系统\n");
    printf("请选择审核项目\n");
    printf("**************************************\n");
    printf("* [1] 审核学术论文  [2] 审核大创项目 *\n");
    printf("* [3] 审核竞赛获奖  [4] 返回上级选择 *\n");
    printf("**************************************\n");
    gets(choice_c);
    choice=atoi(choice_c); 
    if(choice==1){
        system("cls");
        admin_verify_paper();
    }
    else if(choice==2){
        system("cls");
        admin_verify_innovate();
    }
    else if(choice==3){
        system("cls");
        admin_verify_competition();
    }
    else if(choice==4){
        system("cls");
        admin_login();
    }
    else{
        printf("请输入正确的选择\n");
        system("pause");
        system("cls");
        admin_verify_GPA_add();
    }
}

//教务转专业学籍异动管理 可以添加学生 或者删除学生
struct student* Find_change_stu(struct major* head ,char origin_major[100], char change_stu_ID[100])
{
    if(head == NULL)
    {
        printf("专业为空 发生错误\n");
        return NULL;
    }

    struct major* curr_major = head;
    while(curr_major != NULL)
    {
        if(strcmp(origin_major, curr_major->name) == 0)
        {
            break;
        }
        curr_major = curr_major->next;
    }
    if(curr_major == NULL)
    {
        printf("未找到 %s 专业 发生错误\n", origin_major);
        return NULL;
    }  
    else
    {
        struct student* find_stu = curr_major->student_linked_list;
        struct student* prev_stu = NULL;
        if(find_stu == NULL)
        {
            printf("原专业学生信息为空 发生错误\n");
            return NULL;
        }
        while(find_stu != NULL)
        {
            if(strcmp(change_stu_ID, find_stu->student_ID) == 0)
            {
                break;
            }
            prev_stu = find_stu;
            find_stu = find_stu->next;
        }
        if(find_stu == NULL)
        {
            printf("未找到学号为 %s 的学生 发生错误\n", change_stu_ID);
            return NULL;
        }  
        else
        {
            if(prev_stu == NULL)  //  头
            {
                curr_major->student_linked_list = find_stu->next;
            }
            else if(find_stu->next != NULL)    // 中间
            {
                prev_stu->next = find_stu->next;
            }
            else   // 尾
            {
                prev_stu->next = NULL;
            }
            find_stu->next = NULL;  // 断开下一个!!!
            return find_stu; 
        }  
    }
}

//转专业学生处理
void change_stu_major(struct major* head ,char origin_major[100], char new_major[100], char change_stu_ID[100])
{
    struct student* change_stu = Find_change_stu(head ,origin_major, change_stu_ID);
    if(change_stu == NULL)
    {
        printf("调整学号为 %s 的转专业学生失败\n", change_stu_ID);
        return ;
    }
    else
    {
        struct major* curr_major = head;
        while(curr_major != NULL)
        {
            if(strcmp(new_major, curr_major->name) == 0)
            {
                break;
            }
            curr_major = curr_major->next;
        }
        if(curr_major == NULL)
		{
			printf("未找到 %s 专业, 调整学号为 %s 的转专业学生失败!\n", new_major, change_stu_ID);
			return;
		}  
        else
        {	
        	struct student* old = curr_major->student_linked_list;
        	
    		printf("%s 专业原有学生为:\n", curr_major);
    		while(old != NULL)
    		{
				printf("%s\n", old->name);
				old = old->next;
			}
            struct student* find_pos = curr_major->student_linked_list;   // 遍历到最后位置
            if(find_pos == NULL)
            {
                curr_major->student_linked_list = change_stu;
                return ;
            }
            while(find_pos->next != NULL)
            {
                find_pos = find_pos->next;
            }
            find_pos->next = change_stu;
            strcpy(change_stu->major, new_major);
	        printf("\n");
	        printf("目前 学号 %s 姓名 %s 同学 已转入 %s 专业\n", change_stu->student_ID,change_stu->name,change_stu->major); 
	        printf("\n");
	    	printf("%s 专业目前学生为:\n", new_major);
	    	struct student* res = curr_major->student_linked_list;
	    	while(res != NULL)
	    	{
				printf("%s\n", res->name);
				res = res->next;
			}
        }
	}  
}

//教务进行转专业处理
void admin_major_change(struct major* head)
{
	printf("欢迎登入转专业系统!\n");
	char origin_major[100];
	while(1){
	    printf("请输入转专业学生的原专业名称:\n");
	    gets(origin_major);
	    if(strlen(origin_major)==0)
	    	printf("原专业不允许为空 请重新输入\n");
	    else
	    	break;
	}
	char new_major[100];
	while(1){
	    printf("请输入转专业学生的新专业名称:\n");
	    gets(new_major);
	    if(strlen(new_major)==0)
	    	printf("新专业不允许为空 请重新输入\n");
	    else
	    	break;
	}
	char change_stu_ID[100];
	while(1){ 
	    printf("请输入转专业学生的学号:\n");
	    gets(change_stu_ID);
	    if(strlen(change_stu_ID)==0)
	    	printf("转专业学生学号不允许为空 请重新输入\n");
	    else
	    	break;
	}
    change_stu_major(head, origin_major, new_major, change_stu_ID);	
    printf("操作结束 即将返回上一级\n");
    system("pause");
    system("cls");
    admin_login();
}

//论文链表插入操作
void InsertList()
{
    struct paper_node* new_node = (struct paper_node*)malloc(sizeof(struct paper_node));
    char author_num_string[100];
    int author_num_string_res=0;
	while(1)
    {
        printf("请输入要添加的论文作者数量(不超过十个人):\n");
        printf("注意 本系统视最后一名作者为通讯作者\n"); 
        gets(author_num_string);
        author_num_string_res = atoi(author_num_string);
        if(author_num_string_res > 10)
            printf("您输入的作者数量超过上限10 请重新输入\n");
        else if(author_num_string_res <= 0)
        	printf("您输入的作者数量不合法 请重新输入\n");
        else
        {
            printf("请按顺序输入作者名称(以回车分隔):\n");
            for(int i = 0; i < author_num_string_res; i++)
            {
            	while(1){
                	gets(new_node->author[i]);
                	if(strlen(new_node->author[i])==0)
						printf("作者不允许为空 请重新输入\n");
					else
						break; 
            	}
            }
            for(int i=author_num_string_res;i<10;i++){
            	new_node->author[i][0] = '\0';
			}
            printf("您输入的结果如下:\n");
            for(int i = 0; i < author_num_string_res; i++)
            {
                printf("论文第 %d 作者为: %s \n", i+1, new_node->author[i]);
            }
            break;
        }
    }
    char buffer[1000]; 
    while (1) 
    {
        printf("请输入要添加的论文名称:\n");
        gets(buffer);
		if(strlen(buffer)!=0)
        	break;
        else
            printf("输入错误 请重新输入\n");
    }
    strcpy(new_node->paper_name, buffer);
    printf("您输入的论文名称是：%s\n", new_node->paper_name);
    // 输入 论文期刊/会议名称
    while(1){
	    printf("请输入要添加的论文所属期刊/会议名称:\n");
	    gets(new_node->journal_or_conference_name);
	    if(strlen(new_node->journal_or_conference_name)==0)
	    	printf("期刊或会议名称不允许为空 请重新输入\n");
	    else
	    	break;
	}
    printf("您输入的论文所属期刊/会议名称是：%s\n", new_node->journal_or_conference_name);
    char year_c[100];
	char month_c[100];
	char day_c[100];
	while(1)
    {
    	// 输入 论文发布时间
	    printf("请输入要添加的论文发布时间(请以 年 月 日 的顺序输入 并以回车分隔):\n");
	    gets(year_c);
	    new_node->publication_time.year=atoi(year_c);
	    gets(month_c);
	    new_node->publication_time.month=atoi(month_c);
	    gets(day_c);
	    new_node->publication_time.day=atoi(day_c);
	    if(check_time(new_node->publication_time)==0)
	    	printf("您输入的日期不合法 请重新输入\n");
	    else
	    {
	    	printf("您输入的论文发布时间是: %d 年 %d 月 %d 日\n", new_node->publication_time.year, new_node->publication_time.month, new_node->publication_time.day);
	    	break;
		}
	}
    // 输入 论文卷或期或论文号或页码
    char range_c[100];
	while(1){
	    printf("请输入要添加的论文卷或期或论文号或页码:\n");
	    gets(range_c);
	    new_node->range=atoi(range_c);
	    if(new_node->range<=0)
	    	printf("论文卷或期或论文号或页码输入无效 请重新输入\n");
	    else
	    	break;
	}
    printf("您输入的论文卷或期或论文号或页码是: %d\n", new_node->range);
    // 输入 论文对应的级别
    while(1){
	    printf("请输入要添加的论文级别:\n");
		gets(new_node->level);
		if(strlen(new_node->level)==0)
			printf("论文级别不允许为空 请重新输入\n");
		else
			break;
	}
    printf("您输入的论文级别是：%s\n", new_node->level);
	char GPA_add_c[100];
	while(1)
	{
	    printf("请输入要添加的论文对应的素质加分:\n");
	    gets(GPA_add_c);
	    new_node->GPA_add=atof(GPA_add_c);
	    if(new_node->GPA_add == 0) printf("您输入的素质加分不合法 请重新输入\n");
	    else if(new_node->GPA_add > 0.4)
	    {
	    	printf("您输入的论文对应的素质加分 %.2lf 超过 素质加分上限 0.4\n", new_node->GPA_add);
	    	printf("请重新输入\n");
		}
	    else
		{
			printf("您输入的论文对应的素质加分是: %.2lf\n", new_node->GPA_add);
			break;
		} 
	}
    if(paper_head == NULL)
    {
        paper_head = new_node;
        paper_head->next = NULL;
    	printf("初创成功! 即将返回上一级\n");
    	system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
	else
	{
		struct paper_node* temp = paper_head;
        while (temp->next != NULL) {
            temp = temp->next;
		}
		// 将新节点添加到链表的尾部
        temp->next = new_node;
        new_node->next = NULL;
	    printf("创建成功! 即将返回上一级\n");
		system("pause");
		system("cls");
		admin_check_paper();
	    return;
	} 
}

//论文链表删除操作
void DeleteList(char paper[1000])
{
    if(paper_head == NULL)
    {
        printf("当前论文信息情况为空!\n");
        printf("即将返回上一级\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
    if(paper_head->next == NULL)
    {
        if(strcmp(paper, paper_head->paper_name) == 0)
        {
            free(paper_head);
            paper_head = NULL;
            printf("删除成功 即将返回上一级\n");
        	system("pause");
    		system("cls");
    		admin_check_paper();
            return;
        }
        else{
            printf("抱歉! 未找到论文%s的信息\n", paper);
            printf("即将返回上一级\n");
	        system("pause");
	    	system("cls");
	    	admin_check_paper();
	        return;
        }
    }
    struct paper_node* curr = paper_head;
    struct paper_node* prev = NULL;
    while(curr->next != NULL)
    {
        if(strcmp(paper, curr->paper_name) == 0)
        {
            prev->next = curr->next;
            free(curr);
            printf("删除成功，即将返回上一级\n");
        	system("pause");
    		system("cls");
    		admin_check_paper();
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    // last 最后一个结点
    if(strcmp(paper, curr->paper_name) == 0)
    {
        free(curr);
        curr = NULL;
        prev->next=NULL;
        printf("删除成功 即将返回上一级\n");
    	system("pause");
		system("cls");
		admin_check_paper();
    }
    else
    {
        printf("抱歉 未找到论文 %s 的信息", paper);
        printf("即将返回上一级\n");
    	system("pause");
		system("cls");
		admin_check_paper();
        return;
    }
}

//返回论文对应指针 不输出
struct paper_node* Find(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("当前论文信息库为空\n");
        return NULL;
    }
    struct paper_node* curr = head;
    while(curr != NULL)
    {
        if(strcmp(paper, curr->paper_name) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

//查看论文信息
void CheckList(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("当前论文信息库为空\n");
        printf("即将返回上一级\n"); 
        system("pause");
    	system("cls");
    	admin_check_paper();
    	return;
    }  
    struct paper_node* curr = head;
    while(curr != NULL)
    {
        if(strcmp(paper, curr->paper_name) == 0)
        {
            printf("论文名称为: %s\n", paper);
            printf("论文作者为: \n");
            for (int i = 0; i < 10; i++) 
			{
                if (curr->author[i][0] == '\0')
                    break;
                printf("第 %d 作者: %s\n", i + 1, curr->author[i]);
            }
            printf("论文所属期刊名称或会议名称为: %s\n", curr->journal_or_conference_name);
            printf("您的论文发表日期为: %d年 %d月 %d日\n", curr->publication_time.year, curr->publication_time.month, curr->publication_time.day);
            printf("论文卷或期或论文号或页码为: %d\n", curr->range);
            printf("论文级别为: %s\n", curr->level);
            printf("论文素质加分为: %.2lf\n", curr->GPA_add);
	        printf("即将返回上一级\n"); 
	        system("pause");
	    	system("cls");
	    	admin_check_paper();
			return;
        }
        curr = curr->next;
    }
    printf("抱歉 未找到论文 %s 的信息\n", paper);
    printf("即将返回上一级\n");
    system("pause");
    system("cls");
    admin_check_paper();
}

//论文的更新操作
void UpdateList(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("论文 %s 加分情况为空 无可修改文件\n", paper);
        printf("即将返回上一级\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
    struct paper_node* res = Find(head, paper);
    if(res == NULL)
	{
		printf("抱歉 未找到论文 %s 的信息\n", paper);
		printf("即将返回上一级\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
	}  
    else
    {
        printf("请输入您要修改的版块:\n");
        int select = 1;
        while(select)
        {
            printf("***********************************\n");
            printf("* [1] 论文作者  [2] 论文名称  *\n");
            printf("* [3] 期刊名称  [3] 会议名称  *\n");
            printf("* [4] 发表时间  [5] 论文页码  *\n");
            printf("* [6] 论文级别  [7] 素质加分  *\n");
            printf("* [8] 结束修改  [*] ********  *\n");
            printf("***********************************\n");
            printf("请选择:\n");
            char select_c[100];
            gets(select_c);
            select = atoi(select_c);
            if(select == 0)
            {
            	printf("您的输入非法 重新输入\n"); 
            	continue;
			}
            switch(select)
            {
            case 1:
                printf("请输入要修改的作者 从1开始算起:\n");
                int num = 0;
        		char num_c[100]; 
        		gets(num_c);
        		num=atoi(num_c);
        		if(num == 0)
				{
					printf("您的输入非法 请重新输入\n");
					continue;
				}
				while(1){ 
	                printf("请输入要改正后的作者名称: \n");
	                gets(res->author[num-1]);
	                if(strlen(res->author[num-1])==0)
	                	printf("作者名称不允许为空 请重新输入\n");
	                else
	                	break;
            	}
                printf("论文 %s 第 %d 作者被更改为: %s\n", paper, num, res->author[num-1]);
                break;
            case 2:
            	while(1){
	                printf("请输入改正后的论文名称:\n");
	                gets(res->paper_name);
	                if(strlen(res->paper_name)==0)
	                	printf("论文名称不允许为空 请重新输入\n");
	                else
	                	break;
            	}
                printf("论文 %s 名称被更改为: %s\n", paper, res->paper_name);
                break;
            case 3:
            	while(1){
	                printf("请输入改正后的期刊/会议名称:\n");
	                gets(res->journal_or_conference_name);
	                if(strlen(res->journal_or_conference_name)==0)
	                	printf("期刊或会议名称不允许为空 请重新输入\n");
	                else
	                	break;
            	}
                printf("\n论文 %s 期刊/会议名称被更改为: %s\n", paper, res->journal_or_conference_name);
                break;
            case 4:
				while(1){
	                char year_c[100];
					char month_c[100];
					char day_c[100];
					printf("请输入改正后的论文发布时间(请以 年 月 日 的顺序输入):\n");
				    gets(year_c);
				    res->publication_time.year=atoi(year_c);
				    gets(month_c);
				    res->publication_time.month=atoi(month_c);
				    gets(day_c);
				    res->publication_time.day=atoi(day_c);
				    if(check_time(res->publication_time))
				    	break;
				    else
				    	printf("论文发布时间非法 请重新输入\n");
				}
                printf("论文 %s 发表日期被更改为: %d年 %d月 %d日\n", paper, res->publication_time.year, res->publication_time.month, res->publication_time.day);
                break;
            case 5:
                
                while(1){
					char page_c[100];
					printf("请输入改正后的论文页码:\n");
	        		gets(page_c);
	        		res->range=atoi(page_c);
	        		if(res->range<=0)
	        			printf("论文页码非法 请重新输入\n");
	        		else
						break; 
        		}
                printf("论文 %s 页码被更改为: %d\n", paper, res->range);
                break;
            case 6:
            	while(1){
	                printf("请输入改正后的论文级别:>\n");
	                gets(res->level);
	                if(strlen(res->level)==0)
	                	printf("论文级别不允许为空 请重新输入\n");
	                else
	                	break;
            	}
                printf("论文 %s 级别被更改为: %s\n", paper, res->level);
                break;
            case 7:
                
                while(1){
					char gpa_c[100];
					printf("请输入改正后的论文素质加分:\n");
	        		gets(gpa_c);
	        		res->GPA_add=atof(gpa_c);
	        		if(res->GPA_add<=0 || res->GPA_add>0.4)
	        			printf("论文素质加分非法 请重新输入\n");
	        		else
	        			break;
        		}
                printf("论文 %s 素质加分被更改为: %lf\n", paper, res->GPA_add);
                break;
            case 8:
            	select = 0;
                break;
            default:
                printf("输入的命令错误 请重新输入\n");
                break;
            }
        }
        printf("您的修改结束 祝您心情愉快 再见\n");
        printf("即将返回上一级\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
}

//qsort比较函数 按照学业绩点降序排列
int compareAcademicGPA(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->GPA > p2->GPA)   
    return -1;
  else if(p1->GPA < p2->GPA)
    return 1;
  else
    return 0;
}

//qsort比较函数 按照综合绩点降序排列
int compareTotalGPA(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->GPA + p1->GPA_add > p2->GPA + p2->GPA_add)   
    return -1;
  else if(p1->GPA + p1->GPA_add < p2->GPA + p2->GPA_add)
    return 1;
  else
    return 0;
}

// 比较函数，先按照GPA降序排列，如果GPA相同则按照某门课程成绩降序排列
int compareTotalGPA_Plus(const void *a, const void *b) 
{
	struct student *p1 = *(struct student **)a;
  	struct student *p2 = *(struct student **)b;

    // 如果GPA不同，直接比较GPA
    if (p1->GPA + p1->GPA_add != p2->GPA + p2->GPA_add) {
        return (p2->GPA + p2->GPA_add - p1->GPA + p1->GPA_add) > 0 ? 1 : -1;
    }
    // 如果GPA相同，则比较某门课程成绩
    else {
        return (p2->GPA_add - p1->GPA_add);
    }
}

//qsort比较函数 按照平均成绩降序排列
int compareAvgScore(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->AvgScore > p2->AvgScore)   
    return -1;       // qsort默认升序排序  左 小于 右 返回 -1  这里我们想要降序 故应该反过来
  else if(p1->AvgScore < p2->AvgScore)
    return 1;
  else
    return 0;
}

//qsort对某一门课成绩进行比较
int compareProgram(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->score_linked_list->score > p2->score_linked_list->score)   // 这里可能会有问题 可能比较的是成绩链表的第一门课程
    return -1;       // qsort默认升序排序  左 小于 右 返回 -1  这里我们想要降序 故应该反过来
  else if(p1->score_linked_list->score < p2->score_linked_list->score)
    return 1;
  else
    return 0;
}

//计算链表的长度
int getLength(struct student* head)
{
    int len = 0;
    while (head != NULL)
    {
        head = head->next;
        len++;
    }
    return len;
}

//归并排序比较函数 按照GPA排序
int MergeCompareAcademicGPA(struct student *a, struct student *b) {
    if (a->GPA < b->GPA )
        return -1;
    else if (a->GPA > b->GPA )
        return 1;
    else
        return 0;
}

//归并排序 按照GPA+素质加分排序
int MergeCompareTotalGPA(struct student *a, struct student *b) {
    if (a->GPA + a->GPA_add < b->GPA + b->GPA_add)
        return -1;
    else if (a->GPA + a->GPA_add > b->GPA + b->GPA_add)
        return 1;
    else
        return 0;
}

int MergeCompareTotalGPA_Plus(struct student *a, struct student *b) {
    // 如果GPA不同，直接比较GPA
    if (a->GPA + a->GPA_add != b->GPA + b->GPA_add) 
	{
        return (b->GPA + b->GPA_add - a->GPA + a->GPA_add) > 0 ? -1 : 1;     // 和 qsort不一样 
    }
    // 如果GPA相同，则比较某门课程成绩
    else {
        return (b->GPA_add - a->GPA_add);
    }
}

//归并排序
struct student* cut(struct student* node, int n)
{
    struct student* pre = NULL;
    while (n > 0 && node != NULL)
    {
        pre = node;
        node = node->next;
        n--;
    }
    if(node == NULL)
    {
        return NULL;
    }
    pre->next = NULL;
    return node;
}

//merge归并 返回新链的头节点
struct student* merge(struct student* node1, struct student* node2, int (*compare)(struct student*, struct student*))
{
    struct student* dummyHead = (struct student*)malloc(sizeof(struct student));
    struct student* p = dummyHead;
    while (node1 != NULL && node2 != NULL)
    {

        if (compare(node1, node2) >= 0) 
        {
            p->next = node1;
            node1 = node1->next;
        } 
        else 
        {
            p->next = node2;
            node2 = node2->next;
        }

        p = p->next;
    }
    p->next = (node1 != NULL ? node1 : node2);
    struct student* head = dummyHead->next;
    return head;
}

//单链表的归并排序
struct student* ListMergeSort(struct student* head, int (*compare)(struct student*, struct student*)) 
{
    int len = getLength(head);
    struct student* dummyHead = (struct student*)malloc(sizeof(struct student));
    dummyHead->next = head;
    for(int i = 1; i < len; i *= 2)
    {
        struct student* tail = dummyHead;
        struct student* start = dummyHead->next;
        while (start != NULL)
        {
            struct student* L = start;
            struct student* R = cut(L, i);
            start = cut(R, i);
            tail->next = merge(L, R, compare);
            while (tail->next != NULL)
            {		
                tail = tail->next;
            }
        }
    }
    struct student* t = dummyHead->next;
    return t;
}

//综合绩点排名 归并
void MergeRank_with_TotalGPA(struct student* head)
{
	struct student*tmp=head;
	while(tmp!=NULL){
		tmp->GPA_add=cal_GPA_add(tmp);
		tmp=tmp->next;
	}
    struct student *p = ListMergeSort(head, MergeCompareTotalGPA_Plus);
    int num = getLength(p);

	FILE* fp;
	fp = fopen("admin_rank_total_Merge.txt", "w");
	fprintf(fp,"排名\t\t学号\t\t\t姓名\t\t学业绩点\t\t素质类项目加分\t综合绩点\t\t专业\n");
    for(int i = 1; i <= num; i++)
    {
    	if(strlen(p->name)==6)
        	fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i, p->student_ID, p->name, p->GPA, p->GPA_add, p->GPA+p->GPA_add, p->major);
        else
        	fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i, p->student_ID, p->name, p->GPA, p->GPA_add, p->GPA+p->GPA_add, p->major);
        p = p->next;
    }
	fclose(fp);
	printf("综合绩点推免排名已发布\n");
}

//对于指定学生 计算其GPA_add 
double cal_GPA_add(struct student*tmp){
	double sum_GPA_add=0;
	//论文 只计算其第一作者
	struct paper_node*tmp_paper=paper_head; 
	while(tmp_paper!=NULL){
		if(strcmp(tmp_paper->author[0],tmp->name)==0)
			sum_GPA_add+=tmp_paper->GPA_add;
		tmp_paper=tmp_paper->next;
	}
	//大创 第一负责人0.1 第二负责人0.05 
	struct innovate_node*tmp_innovate=innovate_head;
	while(tmp_innovate!=NULL){
		if(strcmp(tmp_innovate->member[0],tmp->name)==0)
			sum_GPA_add+=0.1;
		else if(strcmp(tmp_innovate->member[1],tmp->name)==0)
			sum_GPA_add+=0.05;
		tmp_paper=tmp_paper->next;
	}
	//竞赛 只加前三个人 
	struct competition_node*tmp_competition=competition_head;
	while(tmp_competition!=NULL){
		if(strcmp(tmp_competition->awardee[0],tmp->name)==0)
			sum_GPA_add+=tmp_competition->GPA_add;
		else if(strcmp(tmp_competition->awardee[1],tmp->name)==0)
			sum_GPA_add+=tmp_competition->GPA_add;
		else if(strcmp(tmp_competition->awardee[2],tmp->name)==0)
			sum_GPA_add+=tmp_competition->GPA_add;
		tmp_paper=tmp_paper->next;
	}
	return sum_GPA_add;
}

//学业绩点排名 qsort
void rank_with_AcademicGPA(struct student* head, int num)
{
	struct student*tmp=head; 
	while(tmp!=NULL){
		tmp->GPA_add=cal_GPA_add(tmp);
		tmp=tmp->next;
	}
    struct student **list = (struct student **)malloc(sizeof(struct student *) * num);
    struct student *p = head;
    for(int i = 0; i < num; i++)
    {
        list[i] = p;
        p = p->next;
    }
    qsort(list, num, sizeof(struct student*), compareAcademicGPA);
	FILE* fp;
	fp = fopen("admin_rank_academic_Qsort.txt", "w");
	fprintf(fp,"排名\t\t学号\t\t\t姓名\t\t学业绩点\t\t素质类项目加分\t综合绩点\t\t专业\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	printf("学业绩点排名已发布\n");
    free(list);
}

void rank_with_TotalGPA_Plus(struct student* head, int num)
{
	struct student*tmp=head; 
	while(tmp!=NULL){
		tmp->GPA_add=cal_GPA_add(tmp);
		tmp=tmp->next;
	}
    struct student **list = (struct student **)malloc(sizeof(struct student *) * num);
    struct student *p = head;
    for(int i = 0; i < num; i++)
    {
        list[i] = p;
        p = p->next;
    }
    qsort(list, num, sizeof(struct student*), compareTotalGPA_Plus);
	FILE* fp;
	fp = fopen("admin_rank_total_Qsort.txt", "w");
	fprintf(fp,"排名\t\t学号\t\t\t姓名\t\t学业绩点\t\t素质类项目加分\t综合绩点\t\t专业\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	printf("综合绩点排名已发布\n");
    free(list);
}

//课程排名 qsort
void rank_with_Program(struct student* head, int num)
{
	struct student*tmp=head;
	while(tmp!=NULL){
		tmp->GPA_add=cal_GPA_add(tmp);
		tmp=tmp->next;
	}
    struct student **list = (struct student **)malloc(sizeof(struct student *) * num);
    struct student *p = head;
    for(int i = 0; i < num; i++)
    {
        list[i] = p;
        p = p->next;
    }

    qsort(list, num, sizeof(struct student*), compareProgram);

	FILE* fp;
	fp = fopen("admin_rank_program_Qsort.txt", "w");
	fprintf(fp,"课程: %s\n", list[0]->score_linked_list->subject_name);
	fprintf(fp,"排名\t\t学号\t\t\t姓名\t\t课程成绩\t学业绩点\t\t综合绩点\t\t专业\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.1lf\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name,list[i]->score_linked_list->score, list[i]->GPA,  list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.1lf\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name,list[i]->score_linked_list->score, list[i]->GPA,  list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}

	fclose(fp);
	
	printf("课程排名已发布\n");

    free(list);
}

//对于指定学生 计算其 平均学业成绩 
double cal_AvgScore(struct student*tmp){
	double sum_Score=0;
	int cnt = 0;
	double sum_Credit = 0;
	struct score_node* head = tmp->score_linked_list;
	if(tmp == NULL || head == NULL) return 0;
	while(head != NULL)
	{
		sum_Credit += head->credit;
		sum_Score += head->score * head->credit;
		cnt++;
		head = head->next;
	}
	return sum_Score / sum_Credit;
}

//综合绩点排名 qsort
void rank_with_AvgScore(struct student* head, int num)
{
	struct student*tmp=head;
	while(tmp!=NULL){
		tmp->GPA_add=cal_GPA_add(tmp);
		tmp->AvgScore = cal_AvgScore(tmp);
		
		tmp=tmp->next;
	}
    struct student **list = (struct student **)malloc(sizeof(struct student *) * num);
    struct student *p = head;
    for(int i = 0; i < num; i++)
    {
        list[i] = p;
        p = p->next;
    }

    qsort(list, num, sizeof(struct student*), compareAvgScore);

	FILE* fp;
	fp = fopen("admin_rank_AvgScore_Qsort.txt", "w");
	fprintf(fp,"排名\t\t学号\t\t\t姓名\t\t平均成绩\t\t综合绩点\t\t专业\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->AvgScore, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->AvgScore, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	
	printf("平均成绩排名已发布\n");

    free(list);
}

//教务计算推免成绩并公布排名
void admin_calculate_GPA_added(struct major* major_head)
{
    if(major_head == NULL)
    {
        printf("所有专业为空 计算推免排名失败\n");
        printf("即将返回上级\n");
        system("pause");
        system("cls");
        admin_login();
        return;
    }
	char checkMajor[100];
	while(1){
	    printf("请输入要查询专业排名的专业名称:\n"); 
	    gets(checkMajor);
	    if(strlen(checkMajor)==0)
	    	printf("专业名称不允许为空 请重新输入\n");
	    else
	    	break;
	}
    while(major_head != NULL)
    {
        if(strcmp(major_head->name, checkMajor) == 0)
        {
            break;
        }
        major_head = major_head->next;
    }
    if(major_head == NULL)
    {
        printf("未找到 %s 专业信息 计算推免排名失败\n", checkMajor);
        printf("即将返回上级\n");
        system("pause");
        system("cls");
        admin_login(); 
        return;
    }

	if(major_head->is_published == 0)
    {
        printf(" %s 专业推免排名未发布 查看推免排名失败\n", major_head->name);
        printf("请选择是否发布推免排名: (1)否 (2)是 \n"); 
		int choice = 0;
		char choice_c[100]; 
		gets(choice_c);
		choice=atoi(choice_c);
		if(choice == 1) 
		{
			printf("即将返回上级\n");
	        system("pause");
	        system("cls");
	        admin_login(); 
			return;
		}
		else if(choice == 2)
		{
			printf("正在计算推免排名中 请耐心等待\n");
			printf("\n");
		}
		else{
			printf("输入无效选择 请重新输入\n");
			system("pause");
			system("cls");
			admin_calculate_GPA_added(major_head);
			return;
		}
		
    }
    else if(clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s 专业推免排名已过期 查看推免排名失败\n", major_head->name);
        printf("请选择是否发布推免排名: (1)否 (2)是 \n"); 
        major_head->is_published = 0;
		int choice = 0;
		char choice_c[100]; 
		gets(choice_c);
		choice=atoi(choice_c);
		if(choice == 1) 
		{
			printf("即将返回上级\n");
	        system("pause");
	        system("cls");
	        admin_login();
			return;
		}
		else if(choice == 2)
		{
			printf("正在计算推免排名中 请耐心等待\n");
			printf("\n");
		}
		else{
			printf("输入无效选择 请重新输入\n");
			system("pause");
			system("cls");
			admin_calculate_GPA_added(major_head);
			return;
		}
    }

    struct student*head = major_head->student_linked_list;
    struct student*tmp=head;
	int num=getLength(head);
	
    rank_with_Program(head, num);
    rank_with_AvgScore(head, num); 
    rank_with_AcademicGPA(head, num);
    //rank_with_TotalGPA_Plus(head, num);
	MergeRank_with_TotalGPA(tmp);

    major_head->is_published = 1;
    major_head->publish_time=clock();
    printf("\n专业推免排名已发布 有效时间1分钟 请查看附件\n");
	
    printf("即将返回上级\n");
    system("pause");
    system("cls");
    admin_login(); 
    return;
}

//学生查询推免排名情况 根据目前是否发布进行处理
void student_find_rank_M(struct major* major_head, char*name)
{  
    if(major_head->is_published == 0)
    {
        printf(" %s 专业排名未发布 查看推免排名失败", major_head->name);
        return;
    }
    else if(clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s 专业推免排名公示已过期 查看推免排名失败", major_head->name);
        return;
    } 
    struct student* head = major_head->student_linked_list;
    if(head == NULL) 
    {
        printf("当前专业人数为空 查询失败\n");
        return;
    } 
    int cnt = 0;
    if(major_head->is_published == 1 && clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)   //专业推免排名发布 且 排名未过期
    {
        int flag = 0;// 标志是否找到该学生
        while(head != NULL)
        {
            if(strcmp(head->name, name) == 0)
            {
                flag = 1;
                cnt++;
                break;
            }
            cnt++;
        }
        if(flag == 1)
        {
            printf("排名\t\t学号\t\t\t姓名\t\t学业绩点\t\t素质类项目加分\t综合绩点\t\t专业\n"); 
	    	if(strlen(head->name)==6)
				printf("%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", cnt, head->student_ID, head->name, head->GPA, head->GPA_add, head->GPA+head->GPA_add, head->major);
			else
				printf("%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", cnt, head->student_ID, head->name, head->GPA, head->GPA_add, head->GPA+head->GPA_add, head->major);
        }
        else    printf("未找到该学生信息!");
    }
}

//学生查询 绩点排名 qsort
void student_find_rank(struct major* major_head, char*name)
{
    if(major_head->is_published == 0)// 专业排名未发布
    {
        printf(" %s 专业排名未发布 查看推免排名失败\n", major_head->name);
        printf("即将返回上层\n");
		system("pause");
		student_login();
		return;
    }
	// int publish_time;专业推免排名 剩余 公示时长
    else if(clock()-major_head->publish_time>60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s 专业推免排名公示已过期 查看推免排名失败\n", major_head->name);
        printf("即将返回上层\n");
		system("pause");
		student_login();
		return;
    } 
    struct student* head = major_head->student_linked_list;
    if(head == NULL) 
    {
        printf("当前专业人数为空 查询失败\n");
        printf("即将返回上层\n");
		system("pause");
		student_login();
		return;
    }
    int num = getLength(head);
	//专业推免排名发布
    if(major_head->is_published == 1 && major_head->publish_time > 0)
    {
        struct student **list = (struct student **)malloc(sizeof(struct student *) * num);
        struct student *p = head;
        for(int i = 0; i < num; i++)
        {
            list[i] = p;
            p = p->next;
        }

        qsort(list, num, sizeof(struct student*), compareTotalGPA);
        printf("排名\t学号\t\t姓名\t学业绩点\t素质类项目加分\t\t综合绩点\t专业\n");
        int cnt = 0;
        int flag = 0;
        for(int i = 0; i < num; i++)
        {
            if(strcmp(list[i]->name, name) == 0)
            {
                flag = 1;
                cnt++;
                break;
            }
            cnt++;
        }
        if(flag == 1)  printf("%d\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t%.4lf\t\t%s\n", cnt, list[cnt-1]->student_ID, list[cnt-1]->name, list[cnt-1]->GPA, list[cnt-1]->GPA_add, list[cnt-1]->GPA+list[cnt-1]->GPA_add, list[cnt-1]->major);
        else printf("未找到该学生信息!\n");
        free(list);
    }
    printf("即将返回上层\n");
	system("pause");
	student_login();
	return;
}

// 在竞赛链表中录入新的竞赛
void addCompetition(char competition_name[], char organizer[], char award_level[], int num,char awardee[][100], struct time award_time) {
    // 创建新的竞赛节点
    struct competition_node* new_competition = (struct competition_node*)malloc(sizeof(struct competition_node));
    if (new_competition == NULL) {
        printf("内存分配失败 无法添加竞赛\n");
        return;
    }
    // 设置新竞赛节点的参数
    strcpy(new_competition->competition_name, competition_name);
    strcpy(new_competition->organizer, organizer);
    strcpy(new_competition->award_level, award_level);
    for (int i = 0; i < num; i++) {
        strcpy(new_competition->awardee[i], awardee[i]);
    }
    for(int i=num;i<10;i++){
    	new_competition->awardee[i][0]='\0';
	}
    new_competition->award_time = award_time;
    new_competition->next = NULL;
    // 如果竞赛链表为空，则将新节点设置为头节点
    if (competition_head == NULL) {
        competition_head = new_competition;
        printf("竞赛 %s 添加成功\n", competition_name);
        return;
    }

    // 否则 在链表末尾插入新的竞赛节点
    struct competition_node* temp = competition_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_competition;

    printf("竞赛 %s 添加成功\n", competition_name);
}

//比较两个二维数组是否相等
int compareAwardees(char arr1[][100], char arr2[][100], int size) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(arr1[i], arr2[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

// 删除竞赛链表中的竞赛
//添加awardee_count获奖人数
void deleteCompetition( char competition_name[], char awardee[][100], int AWARDEE_COUNT) {
    struct competition_node* temp = competition_head;
    struct competition_node* prev = NULL;

    // 如果要删除的竞赛是头节点
    if (temp != NULL && strcmp(temp->competition_name, competition_name) == 0 && compareAwardees(temp->awardee, awardee, AWARDEE_COUNT)) {
        competition_head = temp->next;
        free(temp);
        printf("竞赛 %s 删除成功\n", competition_name);
        return;
    }

    // 找到要删除的竞赛
    while (temp != NULL && (strcmp(temp->competition_name, competition_name) != 0 || !compareAwardees(temp->awardee, awardee, AWARDEE_COUNT))) {
        prev = temp;
        temp = temp->next;
    }

    // 如果未找到竞赛
    if (temp == NULL) {
        printf("未找到学生 %s 所对应的竞赛 %s\n", awardee, competition_name);
        return;
    }

    // 从链表中删除竞赛
    prev->next = temp->next;
    free(temp);
    printf("竞赛 %s 删除成功\n", competition_name);
}

// 修改竞赛链表中的竞赛获奖等级
//指向竞赛链表头部的指针 head，要修改的竞赛名称 competition_name，以及新的奖项等级 new_award_level
void modifyCompetition(char competition_name[], char awardee[][100],int num, char new_award_level[]) {
    struct competition_node* temp = competition_head;
    // 找到要修改的竞赛
    while (temp != NULL) {
    	if(strcmp(temp->competition_name, competition_name)==0 && compareAwardees(temp->awardee, awardee, num)){
			break; 
		}
        temp = temp->next;
    }
    // 如果未找到竞赛
    if (temp == NULL) {
        printf("未找到竞赛 %s 修改失败\n", competition_name);
        return;
    }
    // 修改竞赛的奖项等级
    strcpy(temp->award_level, new_award_level);
    printf("竞赛 %s 的奖项等级修改成功\n", competition_name);
}


//查询竞赛链表中的竞赛及获奖者
void queryCompetition(char competition_name[]) {
    struct competition_node* temp = competition_head;
    int found = 0; // 标志是否找到竞赛
    // 遍历竞赛链表，查找指定名称的竞赛
    while (temp != NULL) {
        if (strcmp(temp->competition_name, competition_name) == 0) {
            found = 1;
            // 显示竞赛的详细信息
            printf("竞赛名称: %s\n", temp->competition_name);
            printf("主办方: %s\n", temp->organizer);
            printf("奖项等级: %s\n", temp->award_level);
            printf("获奖时间: %d-%02d-%02d\n", temp->award_time.year, temp->award_time.month, temp->award_time.day);
            printf("获奖者:\n");
            for (int i = 0; i < 10; ++i) {
            	if(temp->awardee[i][0]=='\0')
            		break;
                printf("%d. %s\n", i + 1, temp->awardee[i]);
            }
            printf("\n"); // 添加空行分隔不同的竞赛信息
        }
        temp = temp->next;
    }

    // 如果未找到指定名称的竞赛
    if (!found) {
        printf("未找到竞赛 %s\n", competition_name);
    }
}

// 添加大创项目
void addInnovate( char project_name[], char code[],int num, char member[][100], char instructor[], struct time initiation_time, struct time completion_time) {//struct innovate_node* head_ref
    // 创建新的大创项目节点
    struct innovate_node* new_innovate = (struct innovate_node*)malloc(sizeof(struct innovate_node));
    if (new_innovate == NULL) {
        printf("内存分配失败 无法添加大创项目\n");
        return;
    }

    // 设置新大创项目节点的参数
    strcpy(new_innovate->project_name, project_name);
    strcpy(new_innovate->code, code);
    strcpy(new_innovate->instructor, instructor);
    for (int i = 0; i < num; i++) {
        strcpy(new_innovate->member[i], member[i]);
    }
    for(int i=num;i<5;i++){
    	new_innovate->member[i][0]='\0';
	}
    new_innovate->initiation_time = initiation_time;
    new_innovate->completion_time = completion_time;
    new_innovate->next = NULL;

    // 将新节点添加到大创项目链表
    if (innovate_head == NULL) {
        innovate_head = new_innovate;
        printf("大创项目 %s 添加成功\n", project_name);
        return;
    }

    struct innovate_node* temp = innovate_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_innovate;

    printf("大创项目 %s 添加成功\n", project_name);
}

//删除大创项目
void deleteInnovate(char project_name[]) {
    struct innovate_node* temp = innovate_head;
    struct innovate_node* prev = NULL;

    // 检查链表头节点是否是要删除的节点
    if (temp != NULL && strcmp(temp->project_name, project_name) == 0) {
        innovate_head = temp->next;
        free(temp);
        printf("大创项目 %s 删除成功\n", project_name);
        return;
    }

    // 遍历链表找到要删除的节点
    while (temp != NULL && strcmp(temp->project_name, project_name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // 如果找到要删除的节点，则删除它
    if (temp != NULL) {
        prev->next = temp->next;
        free(temp);
        printf("大创项目 %s 删除成功\n", project_name);
    }
    else {
        printf("未找到大创项目 %s\n", project_name);
    }
}

// 修改大创项目
void modifyInnovate(struct innovate_node* head, char project_name[], char new_project_name[], char new_code[], int num,char new_member[][100], char new_instructor[], struct time new_initiation_time, struct time new_completion_time) {
    struct innovate_node* temp = head;

    // 找到要修改的大创项目节点
    while (temp != NULL && strcmp(temp->project_name, project_name) != 0) {
        temp = temp->next;
    }

    // 如果找到要修改的节点，则进行修改
    if (temp != NULL) {
        strcpy(temp->project_name, new_project_name);
        strcpy(temp->code, new_code);
        strcpy(temp->instructor, new_instructor);
        for (int i = 0; i < num; i++) {
            strcpy(temp->member[i], new_member[i]);
        }
        for(int i=num;i<5;i++){
        	temp->member[i][0]='\0';
		}
        temp->initiation_time = new_initiation_time;
        temp->completion_time = new_completion_time;
        printf("大创项目 %s 修改成功\n", project_name);
    }
    else {
        printf("未找到大创项目 %s\n", project_name);
    }
}

// 查询大创项目
void queryInnovate(struct innovate_node* head, char project_name[]) {
    struct innovate_node* temp = head;
    int found = 0;

    // 遍历链表查找指定名称的大创项目
    while (temp != NULL) {
        if (strcmp(temp->project_name, project_name) == 0) {
            found = 1;
            // 输出大创项目的详细信息
            printf("大创项目名称: %s\n", temp->project_name);
            printf("项目编号: %s\n", temp->code);
            printf("指导教师: %s\n", temp->instructor);
            printf("立项时间: %d-%02d-%02d\n", temp->initiation_time.year, temp->initiation_time.month, temp->initiation_time.day);
            printf("结项时间: %d-%02d-%02d\n", temp->completion_time.year, temp->completion_time.month, temp->completion_time.day);
            printf("项目成员:\n");
            for (int i = 0; i < 5; i++) {
            	if(temp->member[i][0] == '\0')
            		break;
                printf("%d. %s\n", i + 1, temp->member[i]);
            }
            printf("\n");
        }
        temp = temp->next;
    }

    // 输出未找到大创项目的消息
    if (!found) {
        printf("未找到大创项目 %s\n", project_name);
    }
}

void admin_check_paper()
{
	printf("请输入您的选择:\n");
    printf("**************************************\n");
    printf("* [1] 创建论文项目  [2] 修改论文信息 *\n");
    printf("* [3] 删除论文项目  [4] 查询论文项目 *\n");
    printf("* [5] 返回上级选择  [*] ************ *\n");
    printf("**************************************\n");
	int choice1;
    char choice1_c[100]; 
    gets(choice1_c);
    choice1=atoi(choice1_c);
    if (choice1 == 1) {
    	system("cls");
        InsertList();
    }
    else if(choice1 == 2){
    	system("cls");
        char paper[1000];
        while(1){ 
        	printf("请输入所要修改论文项目的名称:\n");
        	gets(paper);
        	if(strlen(paper)==0){
        		printf("论文名称不允许为空 请重新输入\n");
			}
			else{
				break;
			}
    	}
        UpdateList(paper_head, paper);
    }
    else if (choice1 == 3) {
    	system("cls");
        char paper[1000];
        while(1){
	        printf("请输入所要删除论文项目的名称:\n");
	        gets(paper);
	        if(strlen(paper)==0)
	        	printf("论文名称不允许为空 请重新输入\n");
	        else
				break; 
    	}
        DeleteList(paper);
    }
    else if (choice1 == 4) {
    	system("cls");
        char paper[1000];
        while(1){
	        printf("请输入所要查询论文项目的名称:\n");
	        gets(paper);
	        if(strlen(paper)==0)
	        	printf("论文名称不允许为空 请重新输入\n");
	        else
				break; 
    	}
        CheckList(paper_head, paper);
    }
    else if (choice1 == 5) {
		system("cls");
		admin_check_GPA_add();
		return;
    }
    else {
        printf("您的输入有误 请重新输入\n");
		system("pause");
        system("cls");
        admin_check_paper();
        return;
    }
}

void admin_check_innovate()
{
	printf("请输入您的选择:\n");
    printf("**************************************\n");
    printf("* [1] 创建大创项目  [2] 修改大创等级 *\n");
    printf("* [3] 删除大创项目  [4] 查询大创项目 *\n");
    printf("* [5] 返回上级选择  [*] ************ *\n");
    printf("**************************************\n");
    int choice2;
    char choice2_c[100]; 
    gets(choice2_c);
    choice2=atoi(choice2_c);
    if (choice2 == 1) {
    	system("cls");
    	char project_name[1000];
		while(1){ 
	        printf("请输入要添加的大创项目名称\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("大创项目名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char code[100];
		while(1){
	        printf("请输入项目编号\n");
	        gets(code);
	        if(strlen(code)==0)
	        	printf("项目编号不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char num_c[100];
		int num=0; 
        while (1)
        {	
			printf("请输入项目人数\n");
            gets(num_c);
			num=atoi(num_c);
            if (num > 0 && num <= 5) {
                break;
            }
            else printf("人数输入错误 请重新输入\n");
        }
        printf("请按顺序输入参与人员名称\n");
        char member[5][100];
        for (int i = 0; i < num; i++) {
        	while(1){
        		gets(member[i]);
        		if(strlen(member[i])==0)
					printf("参与人员名称不允许为空 请重新输入\n");
				else
					break; 
			}
        }
		char instructor[100];
		while(1){
	        printf("请输入指导教师名称\n");
	        gets(instructor);
	        if(strlen(instructor)==0)
	        	printf("指导教师名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char year_c[100];
		char month_c[100];
		char day_c[100];
		struct time init_time; 
    	while(1){
	        printf("请按年月日输入起始时间\n");
		    gets(year_c);
		    init_time.year=atoi(year_c);
		    gets(month_c);
		    init_time.month=atoi(month_c);
		    gets(day_c);
		    init_time.day=atoi(day_c);
		    if(check_time(init_time))
		    	break;
		    else
		    	printf("起始时间输入非法 请重新输入\n");
		}
		struct time com_time;
		while(1){
	        printf("请按年月日输入结项时间\n");
		    gets(year_c);
		    com_time.year=atoi(year_c);
		    gets(month_c);
		    com_time.month=atoi(month_c);
		    gets(day_c);
		    com_time.day=atoi(day_c);
		    if(check_time(com_time))
		    	break;
		    else
		    	printf("结项时间输入非法 请重新输入\n");
		}
        addInnovate(project_name, code, num,member, instructor, init_time, com_time);
    	printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
	}
    else if (choice2 == 2) {
    	system("cls");
		char project_name[1000];
		while(1){
	        printf("请输入要修改的大创项目名称\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("项目名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char new_project_name[1000];
        while(1){
			printf("请输入新的大创项目名称\n");
	        gets(new_project_name);
	        if(strlen(new_project_name)==0)
	        	printf("项目名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char code[100];
		while(1){
	        printf("请输入新的项目编号\n");
	        gets(code);
	        if(strlen(code)==0)
	        	printf("项目编号不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		int num = 0;
        char num_c[100];
        printf("请输入新的项目人数\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 5) {
                break;
            }
            else printf("人数错误 请重新输入\n");
        }
        printf("请按顺序输入参与人员名称\n");
        char member[5][100];
        for (int i = 0; i < num; i++) {
        	while(1){ 
            	gets(member[i]);
            	if(strlen(member[i])==0)
            		printf("参与人员不允许为空 请重新输入\n");
            	else
            		break;
        	}
        }
		char instructor[100];
		while(1){
	        printf("请输入指导教师名称\n");
	        gets(instructor);
	        if(strlen(instructor)==0)
	        	printf("指导教师名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char year_c[100];
		char month_c[100];
		char day_c[100];
		struct time init_time;
		while(1){
	        printf("请按年月日输入起始时间\n");
		    gets(year_c);
		    init_time.year=atoi(year_c);
		    gets(month_c);
		    init_time.month=atoi(month_c);
		    gets(day_c);
		    init_time.day=atoi(day_c);
		    if(check_time(init_time))
		    	break;
		    else
		    	printf("起始时间输入非法 请重新输入\n");
		}
		struct time com_time;
		while(1){
	        printf("请按年月日输入结项时间\n");    
		    gets(year_c);
		    com_time.year=atoi(year_c);
		    gets(month_c);
		    com_time.month=atoi(month_c);
		    gets(day_c);
		    com_time.day=atoi(day_c);
		    if(check_time(com_time))
		    	break;
		    else
		    	printf("结项时间输入非法 请重新输入\n");
		}
        modifyInnovate(innovate_head, project_name, new_project_name, code,num,member, instructor, init_time, com_time);
    	printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
    }
    else if (choice2 == 3) {
    	system("cls");
    	char project_name[1000];
    	while(1){ 
	        printf("请输入要删除的大创项目名称\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
				printf("大创项目名称不允许为空 请重新输入\n");
			else
				break; 
    	}
        deleteInnovate(project_name);
    	printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
    }
    else if (choice2 == 4) {
    	system("cls");
		char project_name[1000];
        while(1){
			printf("请输入要查询的大创项目名称\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("大创项目名称不允许为空 请重新输入\n");
			else
				break; 
    	}
        queryInnovate(innovate_head, project_name);
    	printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
    }
    else if (choice2 == 5) {
    	system("cls");
		admin_check_GPA_add();
		return;
    }
    else {
        printf("您的输入有误，请重新输入\n");
        system("pause");
		system("cls");
        admin_check_innovate();
        return;
    }
}

void admin_check_competition(){
	printf("请输入您的选择:\n");
    printf("**************************************\n");
    printf("* [1] 创建竞赛项目  [2] 修改竞赛等级 *\n");
    printf("* [3] 删除竞赛项目  [4] 查询竞赛项目 *\n");
    printf("* [5] 返回上级选择  [*] ************ *\n");
    printf("**************************************\n");
    int choice3;
    char choice3_c[100];
    gets(choice3_c);
    choice3=atoi(choice3_c);
    if (choice3 == 1) {
    	system("cls");
    	char competition_name[1000];
    	while(1){ 
	        printf("请输入要添加的竞赛名称\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
	        	printf("竞赛名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
		char organizer[100];
        while(1){
			printf("请输入主办方\n");
	        gets(organizer);
	        if(strlen(organizer)==0)
	        	printf("主办方不允许为空 请重新输入\n");
	        else
	        	break;
    	}
    	char award_level[100];
    	while(1){
	        printf("请输入奖项等级\n");
	        gets(award_level);
	        if(strlen(award_level)==0)
	        	printf("奖项等级不允许为空 请重新输入\n");
			else
				break; 
    	}
		int num = 0;
        char num_c[100]; 
        printf("请输入竞赛参与人数\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("人数错误 请重新输入\n");
        }
        printf("请按顺序输入参与人员名称\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
        	while(1){
            	gets(awardee[i]);
            	if(strlen(awardee[i])==0)
            		printf("参与人员名称不允许为空 请重新输入\n");
            	else
            		break;
			}
        }
		char year_c[100];
		char month_c[100];
		char day_c[100];
        struct time award_time;
		while(1){
	        printf("请按年月日输入获奖时间\n");
		    gets(year_c);
		    award_time.year=atoi(year_c);
		    gets(month_c);
		    award_time.month=atoi(month_c);
		    gets(day_c);
		    award_time.day=atoi(day_c);
		    if(check_time(award_time))
		    	break;
		    else
		    	printf("获奖时间输入非法 请重新输入\n");
		}
        addCompetition(competition_name, organizer, award_level, num,awardee, award_time);
		printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_competition();
		return; 
    }
    else if (choice3 == 2) {
    	system("cls");
		char competition_name[1000];
        while(1){ 
			printf("请输入要修改的竞赛名称\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
	        	printf("竞赛名称不允许为空 请重新输入\n");
	        else
	        	break;
    	}
    	int num = 0;
        char num_c[100];
        printf("请输入竞赛参与人数\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("人数错误 请重新输入\n");
        }
        printf("请按顺序输入参与人员名称\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
        	while(1){
            	gets(awardee[i]);
            	if(strlen(awardee[i])==0)
            		printf("参与人员名称不允许为空 请重新输入\n");
            	else
            		break;
        	}
        }
		char new_award_level[100];
        while(1){
	        printf("请输入新的获奖等级\n");    
	        gets(new_award_level);
	        if(strlen(new_award_level)==0)
	        	printf("获奖等级不允许为空 请重新输入\n");
	        else
	        	break; 
    	}
        modifyCompetition(competition_name, awardee, num,new_award_level);
		printf("即将返回上级\n");
		system("pause");
		system("cls");
		admin_check_competition();
		return; 
    }
    else if (choice3 == 3) {
    	system("cls");
        printf("请输入要删除的竞赛名称\n");
        char competition_name[100];
        gets(competition_name);
        printf("请输入竞赛参与人数\n");
        int num = 0;
        char num_c[100];
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("人数错误 请重新输入\n");
        }
        printf("请按顺序输入参与人员名称\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
            while(1){
				gets(awardee[i]);
				if(strlen(awardee[i])==0)
					printf("参与人员名称不允许为空 请重新输入\n");
				else
					break;
			}
        }
        deleteCompetition(competition_name, awardee,num);
		system("pause");
		system("cls");
		admin_check_competition();
		return; 

    }
    else if (choice3 == 4) {
    	system("cls");
		char competition_name[1000];
        while(1){
			printf("请输入要查询的竞赛名称\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
				printf("竞赛名称不允许为空 请重新输入\n");
			else
				break; 
    	}
        queryCompetition(competition_name);
		system("pause");
		system("cls");
		admin_check_competition(); 
		return; 
    }
    else if(choice3 == 5){
		system("cls");
		admin_check_GPA_add();
		return;
    }
    else {
        printf("您的输入有误，请重新输入\n");
        system("pause");
        system("cls");
        admin_check_competition();
        return;
    }
}

void admin_check_GPA_add(){
	system("cls");
    printf("欢迎进入素质加分管理系统\n");
    printf("请输入您的选择:\n");
    printf("******************************\n");
    printf("* [1] 论文管理  [2] 大创管理 *\n");
    printf("* [3] 竞赛管理  [4] 返回上级 *\n");
    printf("******************************\n");
    int choice;
    char choice_c[100];
    gets(choice_c);
    choice=atoi(choice_c);
    if(choice==1){
    	system("cls");
        admin_check_paper();       
    }
    else if (choice == 2) {
    	system("cls");
    	admin_check_innovate();
    }
    else if (choice == 3) {
    	system("cls");
    	admin_check_competition();   
    }
    else if (choice == 4) {
    	system("cls");
        admin_login();
        return;
    }
	else{
        printf("请输入正确的选择\n");
        system("pause");
        system("cls");
        admin_check_GPA_add();
        return;
    }
}

//学生登录系统
void student_login(){
	char major[100];
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("请输入学生专业 学号及密码:\n");
        while(1){
        	gets(major);
        	if(strlen(major)==0)
        		printf("专业不允许为空 请重新输入\n");
			else
				break; 
    	}
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("学号输入错误 请重新输入\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("密码不允许为空 请重新输入\n");
        	else
        		break;
    	}
        struct student*stu=student_login_verify(major,ID,password);
        if(stu!=NULL){
                logged_in=1;
				logged_in_stu=stu;
                system("cls");
                printf("学生 %s 登陆成功 请选择执行功能:\n",logged_in_stu->name);
			    printf("*******************************************\n");
			    printf("* [1] 上报素质加分  [2] 查询课程成绩与GPA *\n");
			    printf("* [3] 查询推免排名  [4] 修改个人信息      *\n");
			    printf("* [5] 返回上层选择  [*] ************      *\n");
			    printf("*******************************************\n");
                gets(choice_c);
                choice=atoi(choice_c);
                if(choice==1){
                	system("cls");
                    student_update_GPA_add();
                }
                else if(choice==2){
                	system("cls");
                    student_course_GPA_check(logged_in_stu);
                }
                else if(choice==3){
                	system("cls");
                    student_find_rank(major_head,logged_in_stu->name);
                }
                else if(choice==4){
                	system("cls");
                    student_information_change(logged_in_stu);
                }
                else if(choice==5){
                	system("cls");
                    logged_in=0;
                    logged_in_stu=NULL;
                    screen();
                }
                else{
                    printf("请输入正确的选择\n");
                    system("pause");
                    system("cls");
                    student_login();
                }
            }
        else{
            printf("不存在此专业或学号密码错误 请重新输入\n");
            system("pause");
            system("cls");
            student_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("学生 %s 登陆成功 请选择执行功能:\n",logged_in_stu->name);
	    printf("*******************************************\n");
	    printf("* [1] 上报素质加分  [2] 查询课程成绩与GPA *\n");
	    printf("* [3] 查询推免排名  [4] 修改个人信息      *\n");
	    printf("* [5] 返回上层选择  [*] ************      *\n");
	    printf("*******************************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
        	system("cls");
            student_update_GPA_add();
        }
        else if(choice==2){
        	system("cls");
            student_course_GPA_check(logged_in_stu);
        }
        else if(choice==3){
        	system("cls");
            student_find_rank(major_head,logged_in_stu->name);
        }
        else if(choice==4){
        	system("cls");
            student_information_change(logged_in_stu);
        }
        else if(choice==5){
        	system("cls");
            logged_in=0;
            logged_in_stu=NULL;
            screen();
        }
        else{
            printf("请输入正确的选择\n");
            system("pause");
            system("cls");
            student_login();
        }
    }
}

//教师登录系统
void teacher_login(){
	char major[100]; 
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("请输入教师专业 工号及密码:\n");
        while(1){
        	gets(major);
        	if(strlen(major)==0)
        		printf("专业不允许为空 请重新输入\n");
			else
				break; 
    	}
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("工号输入错误 请重新输入\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("密码不允许为空 请重新输入\n");
        	else
        		break;
    	}
        struct teacher*tea=teacher_login_verify(major,ID,password);
        if(tea!=NULL){
                logged_in=1;
				logged_in_tea=tea;
                system("cls");
                printf("教师 %s 登陆成功 请选择执行功能:\n",logged_in_tea->name);
			    printf("***************************************\n");
			    printf("* [1] 课程成绩登入  [2] 课程成绩管理  *\n");
			    printf("* [3] 修改个人信息  [4] 返回上层      *\n");
			    printf("***************************************\n");
                gets(choice_c);
                choice=atoi(choice_c);
                if(choice==1){
                	system("cls");
                    teacher_course_upload(logged_in_tea);
				}
                else if(choice==2){
                    system("cls");
					teacher_course_manage(logged_in_tea);
                }
                else if(choice==3){
                    system("cls");
					teacher_information_change(logged_in_tea);
                }
                else if(choice==4){
                	system("cls");
                    logged_in=0;
                    logged_in_tea=NULL;
                    screen();
                }
                else{
                    printf("请输入正确的选择\n");
                    system("pause");
                    system("cls");
                    teacher_login();
                }
            }
        else{
            printf("不存在此专业或工号密码错误 请重新输入\n");
            system("pause");
            system("cls");
            teacher_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("教师 %s 登陆成功 请选择执行功能:\n",logged_in_tea->name);
	    printf("***************************************\n");
	    printf("* [1] 课程成绩登入  [2] 课程成绩管理  *\n");
	    printf("* [3] 修改个人信息  [4] 返回上层      *\n");
	    printf("***************************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
            system("cls");
			teacher_course_upload(logged_in_tea);
        }
        else if(choice==2){
            system("cls");
			teacher_course_manage(logged_in_tea);
        }
        else if(choice==3){
            system("cls");
			teacher_information_change(logged_in_tea);
        }
        else if(choice==4){
        	system("cls");
            logged_in=0;
            logged_in_tea=NULL;
            screen();
        }
        else{
            printf("请输入正确的选择\n");
            system("pause");
            system("cls");
            teacher_login();
        }
    }
}

//教务登录系统
void admin_login(){
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("请输入教务工号及密码:\n");
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("工号输入错误 请重新输入\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("密码不允许为空 请重新输入\n");
        	else
        		break;
    	}
        struct admin*adm=admin_login_verify(ID,password);
        logged_in_adm=adm;
        if(adm!=NULL){
                logged_in=1;
                system("cls");
                printf("教务 %s 登陆成功 请选择执行功能:\n",logged_in_adm->name);
			    printf("***************************************\n");
			    printf("* [1] 查看素质加分  [2] 素质加分审核  *\n");
			    printf("* [3] 公布推免排名  [4] 转专业管理    *\n");
			    printf("* [5] 修改个人信息  [6] 返回上层选择  *\n");
			    printf("***************************************\n");
                gets(choice_c);
                choice=atoi(choice_c);
                if(choice==1){
                    system("cls");
					admin_check_GPA_add();
				}
                else if(choice==2){
                    system("cls");
					admin_verify_GPA_add();
                }
                else if(choice==3){
                    system("cls");
					admin_calculate_GPA_added(major_head);
                }
                else if(choice==4){
                    system("cls");
					admin_major_change(major_head);
                }
                else if(choice==5){
                    system("cls");
					admin_information_change(logged_in_adm);
                }
                else if(choice==6){
                	system("cls");
                    logged_in=0;
                    logged_in_adm=NULL;
                    screen();
                }
                else{
                    printf("请输入正确的选择\n");
                    system("pause");
                    system("cls");
                    admin_login();
                }
            }
        else{
            printf("账号或密码错误 请重新输入\n");
            system("pause");
            system("cls");
            admin_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("教务 %s 登陆成功 请选择执行功能:\n",logged_in_adm->name);
	    printf("***************************************\n");
	    printf("* [1] 查看素质加分  [2] 素质加分审核  *\n");
	    printf("* [3] 公布推免排名  [4] 转专业管理    *\n");
	    printf("* [5] 修改个人信息  [6] 返回上层选择  *\n");
	    printf("***************************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
            system("cls");
			admin_check_GPA_add();
        }
        else if(choice==2){
            system("cls");
			admin_verify_GPA_add();
        }
        else if(choice==3){
            system("cls");
			admin_calculate_GPA_added(major_head);
        }
        else if(choice==4){
            system("cls");
			admin_major_change(major_head);
        }
        else if(choice==5){
            system("cls");
			admin_information_change(logged_in_adm);
        }
        else if(choice==6){
        	system("cls");
            logged_in=0;
            logged_in_adm=NULL;
            screen();
        }
        else{
            printf("请输入正确的选择\n");
            system("pause");
            system("cls");
            admin_login();
        }
    }
}

//主函数
int main(){
    system_initialization();
    screen();
    return 0;
}