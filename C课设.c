#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#include<locale.h>
#include"struct.h"

//��¼��ʱ�Ƿ��Ѿ���¼
int logged_in=0;
struct student*logged_in_stu;
struct teacher*logged_in_tea;
struct admin*logged_in_adm;

//�����ʼ��
struct major*major_head=NULL;
struct paper_node*paper_head=NULL;
struct innovate_node*innovate_head=NULL;
struct competition_node*competition_head=NULL;
struct admin*admin_head=NULL;
struct paper_node*paper_process_head=NULL;
struct innovate_node*innovate_process_head=NULL;
struct competition_node*competition_process_head=NULL;

//��������
//����ѧ����Ӧ��GPA_add 
double cal_GPA_add(struct student*tmp);
//����鿴ѧ������ 
void admin_check_paper();
//����鿴����Ŀ 
void admin_check_innovate();
//����鿴������ 
void admin_check_competition();
//����鿴���ʼӷ���Ŀ 
void admin_check_GPA_add();
//ѧ���޸ĸ�����Ϣ �޸�����
void student_information_change(struct student* S);
//��ʦ����γ̳ɼ�
void teacher_course_manage(struct teacher* T);
//��ʦ�޸ĵ�½����
void teacher_information_change(struct teacher* T);
//�����޸ĵ�½����
void admin_information_change(struct admin* A);
//��������Ŀγ̳ɼ� �����Ӧ�ļ���
double find_gpa(double score);
//���ڸ���ѧ�� ������GPA
double calculate_GPA(struct student*stu);
//���ļ�����רҵ��Ϣ
void read_major(FILE*fp);
//���ļ��ж���ѧ����Ϣ
void read_student(FILE*fp);
//���ļ��ж�ȡ��ʦ��Ϣ
void read_teacher(FILE*fp);
//���ļ��ж�ȡ������Ϣ
void read_admin(FILE*fp);
//���ļ��ж���ɼ���Ϣ
void read_score(FILE*fp);
//ϵͳ��ʼ������ ���ȶ������е�ѧ�� ��ʦ�ͽ�����Ϣ
void system_initialization();
//���������� ѡ��ѧ�� ��ʦ���߽�����е�¼
void screen();
//ѧ����¼�˺�������֤
struct student*student_login_verify(char*major,char*ID,char*password);
//��ʦ��¼�˺�������֤
struct teacher*teacher_login_verify(char*major,char*ID,char*password);
//�����¼�˺�������֤
struct admin*admin_login_verify(char*ID,char*password);
//ѧ����ѯ�Լ���GPA
void student_course_GPA_check(struct student* S);
//ѧ���ϱ�ѧ������
void student_update_paper();
//ѧ���ϱ�����Ŀ
void student_update_innovate();
//ѧ���ϱ�������
void student_update_competition();
//ѧ���ϱ�ѧԺ���ʼӷ���� ������ ���ĺ;���
void student_update_GPA_add();
//��ʦ�ϴ��γ̳ɼ�
void teacher_course_upload();
//�������ѧ������
void admin_verify_paper();
//������˴���Ŀ
void admin_verify_innovate();
//������˾�����
void admin_verify_competition();
//����������ʼӷ����
void admin_verify_GPA_add();
//����תרҵѧ���춯���� �������ѧ�� ����ɾ��ѧ��
struct student* Find_change_stu(struct major* head ,char origin_major[100], char change_stu_ID[100]);
//תרҵѧ������
void change_stu_major(struct major* head ,char origin_major[100], char new_major[100], char change_stu_ID[100]);
//�������תרҵ����
void admin_major_change(struct major* head);
//��������������
void InsertList();
//��������ɾ������
void DeleteList(char paper[1000]);
//�������Ķ�Ӧָ�� �����
struct paper_node* Find(struct paper_node* head, char paper[1000]);
//�鿴������Ϣ
void CheckList(struct paper_node* head, char paper[1000]);
//���ĵĸ��²���
void UpdateList(struct paper_node* head, char paper[1000]);
//qsort�ȽϺ��� ����ѧҵ���㽵������
int compareGPA(const void *a, const void *b);
//qsort�ȽϺ��� �����ۺϼ��㽵������
int compareTotalGPA(const void *a, const void *b);
//qsort�ȽϺ��� ����ƽ���ɼ���������
int compareAvgScore(const void *a, const void *b);
//����ƽ��ѧҵ�ɼ� 
double cal_AvgScore(struct student*tmp);
//ƽ��ѧҵ�ɼ����� 
void rank_with_AvgScore(struct student* head, int num);
//qsort��ĳһ�ſγɼ����бȽ�
int compareProgram(const void *a, const void *b);
//��������ĳ���
int getLength(struct student* head);
//�ۺϼ������� qsort
void rank_with_GPA(struct student* head, int num,int choice);
//�����������ɼ�����������
void admin_calculate_GPA_added(struct major* major_head);
//ѡ��רҵ��������������
//void rank_admin(struct major* major_head);
//ѧ����ѯ����������� ����Ŀǰ�Ƿ񷢲����д���
void student_find_rank_M(struct major* major_head, char*name);
//ѧ����ѯ �������� qsort
void student_find_rank(struct major* major_head, char*name);
//ѧ����¼ϵͳ
void student_login();
//��ʦ��¼ϵͳ
void teacher_login();
//�����¼ϵͳ
void admin_login();

int day_of_month[12]={31,29,31,30,31,30,31,31,30,31,30,31};
//����ʱ�����Ч�� 
int check_time(struct time t){
	return (t.year>=2004 && t.year<=2024 && t.month>=1 && t.month<=12 && t.day>=1 && t.day<=day_of_month[t.month-1]);
}

//ѧ����ѯ�Լ��� �ɼ� �� GPA
void student_course_GPA_check(struct student* S) {
	
	double gpa_academic = calculate_GPA(S);
	double avg_score = cal_AvgScore(S);
	
    printf("ѧҵ��Ŀ���� GPA: %.4lf\n", gpa_academic);
    double gpa_add=cal_GPA_add(S);
    printf("������Ŀ���� GPA: %.2lf\n", gpa_add);
    printf("�ۺϳɼ����� GPA: %.4lf\n", gpa_academic+gpa_add);
    printf("ƽ��ѧҵ�ɼ�    : %.4lf\n", avg_score);
    
    printf("\n"); 
	
	struct score_node* res = S->score_linked_list;
	if(res == NULL) 
	{
		printf("ѧ�� %s Ŀǰû�гɼ���Ϣ!\n", S->name);
		printf("\n���������ϲ�\n");
    	system("pause");
    	system("cls");
    	student_login();
    	return;
	}
	printf("--------------------------------------------------------------------------------------------------\n");
	printf("��Ŀ\t\t\t�ɼ�\t\tѧ��\t\t����\t\t����\n");
	printf("--------------------------------------------------------------------------------------------------\n");
	while(res != NULL)
	{
		char model[100];
		if(res->is_required_course == 1)
			strcpy(model, "����");
		else
			strcpy(model, "ѡ��");
		printf("%s\t\t%.1lf\t\t%.1lf\t\t%.1lf\t\t%s\n", res->subject_name, res->score, res->credit, res->grade_point, model);
		res = res->next;
	}
    
    printf("\n���������ϲ�\n");
    system("pause");
    system("cls");
    student_login();
    return;
}

//��ʦ����γ̳ɼ�
void teacher_course_manage(struct teacher* T) {
	system("cls");
    int choice;
    char choice_c[100];
    printf("��ӭ��¼��ʦ�γ̹���ϵͳ ��ѡ����\n");
    printf("***************************************\n");
    printf("* [1] �޸�ѧ���ɼ�  [2] ɾ��ѧ���ɼ�  *\n");
    printf("* [3] �����ϲ�ѡ��  [*] ************  *\n");
    printf("***************************************\n");
    gets(choice_c);
    choice=atoi(choice_c);
    if (choice == 1) {
    	system("cls");
        struct major* p = major_head;
        //�ҵ���ʦ��Ӧ��רҵ 
        while (p != NULL) {
            if (strcmp(p->name, T->major)==0)
                break;
            p = p->next;
        }
        struct student* q = p->student_linked_list;
        char xiugai[100];
        while(1){ 
	        printf("����Ҫ�޸ĳɼ���ѧ��ѧ��:\n");
	        gets(xiugai);
	        if(strlen(xiugai)!=8)
	        	printf("����ѧ�ų��ȴ��� ����������\n");
	        else
	        	break;
    	}
        while (q != NULL) {
            if (strcmp(q->student_ID, xiugai) == 0)
                break;
            q = q->next;
        }
        if (q == NULL) {
            printf("ѧ�Ŵ��� ������\n");
            system("pause");
            teacher_course_manage(T);
            return;
        }
        double score = 0;
		char score_c[100];
        while(1){
	        printf("������γ̵ĳɼ�:\n");
	        gets(score_c);
	        score=atof(score_c);
	        if(score>0 && score<=100)
	        	break;
	        else{
	        	printf("����ɼ����� ����������\n");
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
        	printf("�γ̳ɼ���δ�ϱ� ���������ϼ�\n");
		}
		else{
        printf("�޸ĳɹ� ���������ϼ�\n");
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
        printf("����Ҫɾ���ɼ���ѧ��ѧ��:\n");
        char shanchu[100];
        while(1){
	        gets(shanchu);
	        if(strlen(shanchu)!=8)
	        	printf("ѧ���������� ����������\n");
	        else
	        	break;
    	}
        while (q != NULL) {
            if (strcmp(q->student_ID, shanchu) == 0)
                break;
            q = q->next;
        }
        if (q == NULL) {
            printf("ѧ�Ŵ��� ������\n");
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
        	printf("�����ڴ˿γ� ���������ϼ�\n");
		}
		else{
        	printf("ɾ���ɹ� ���������ϼ�\n");
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
    	printf("��������ȷ��ѡ��\n");
		system("pause");
		system("cls");
		teacher_course_manage(T);
		return;
	}
}

//ѧ���޸ĸ�����Ϣ �޸�����
void student_information_change(struct student*S){
	system("cls");
	printf("��������º������\n"); 
    char change_password[20];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("���벻��Ϊ�� ����������\n");
        system("pause"); 
        student_information_change(S);
    }
    else{
	    printf("�����޸ĳɹ� ���������ϼ�\n");
	    strcpy(S->password,change_password);
	    system("pause");
	    student_login();
	}
    return;
}

//��ʦ�޸ĵ�½����
void teacher_information_change(struct teacher*T){
	system("cls");
	printf("��������º������\n"); 
    char change_password[100];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("���벻��Ϊ�� ����������\n");
        system("pause"); 
        teacher_information_change(T);
    }
    else{
	    printf("�����޸ĳɹ� ���������ϼ�\n");
	    strcpy(T->password,change_password);
	    system("pause");
	    teacher_login();
	}
    return;
}

//�����޸ĵ�½����
void admin_information_change(struct admin*A){
	system("cls");
	printf("��������º������\n"); 
    char change_password[100];
    gets(change_password);
    if(strlen(change_password)==0){
        printf("���벻��Ϊ�� ����������\n");
        system("pause"); 
        admin_information_change(A);
    }
    else{
	    printf("�����޸ĳɹ� ���������ϼ�\n");
	    strcpy(A->password,change_password);
	    system("pause");
	    admin_login();
	}
    return;
}

//��������Ŀγ̳ɼ� �����Ӧ�ļ���
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

//���ڸ���ѧ�� ������GPA
double calculate_GPA(struct student*stu){
    double sum_of_credit=0;
    double sum_of_grade_point=0;
    struct score_node*head=stu->score_linked_list;
    if(head==NULL){
        return 0;
    }
    while(head!=NULL){
        //ѡȡ���޿μ������Ӧ����
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

// ���ļ�����רҵ��Ϣ
void read_major(FILE*fp){
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
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
    printf("�ܼƶ��� %d ��רҵ��Ϣ\n",cnt); 
    fclose(fp);
    return;
}

//���ļ��ж���ѧ����Ϣ
void read_student(FILE*fp){
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
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
    //�������� ѧ���� ���� ���� רҵ
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
        	printf("�ڶ�ȡ�� %d λѧ��ʱ ��������\n",cnt); 
            printf("�����ڴ�רҵ: %s ��ȡ����\n",tmp_major);
            continue;
        }
        //��ʱqָ��ѧ����Ӧ��רҵ ��ѧ�����뵽ѧ�������ĩβ
        p=q->student_linked_list;
        if(p!=NULL){
            while(p->next!=NULL)
                p=p->next;
        }
        //���α���ѧ���� ѧ���� ���� ���� רҵ
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
    printf("�ܼƶ��� %d λѧ����Ϣ\n",cnt);
    fclose(fp);
    return;
}

//���ļ��ж�ȡ��ʦ��Ϣ
void read_teacher(FILE*fp){
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
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
    //���ζ��� ѧ���� ���� ���� ���ڿγ� רҵ �Ƿ���޿� �γ�ѧ��
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
        	printf("�ڶ���� %d λ��ʦʱ ��������\n",cnt);
            printf("�����ڴ�רҵ: %s ��ȡ����\n",tmp_major);
            continue;
        }
        //��ʱqָ���ʦ��Ӧ��רҵ�ڵ� ����ʦ���뵽��ʦ�����ĩβ
        p=q->teacher_linked_list;
        if(p!=NULL){
            while(p->next!=NULL)
                p=p->next;
        }
        //���α����ʦ�� ѧ���� ���� ���� ���ڿγ� רҵ �Ƿ�Ϊ���޿� �γ�ѧ��
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
    printf("�ܼƶ��� %d λ��ʦ��Ϣ\n",cnt); 
    fclose(fp);
    return;
}

//���ļ��ж�ȡ������Ϣ
void read_admin(FILE*fp){
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
        system("pause");
        return;
    }
    int cnt=0;
    struct admin*r;
    struct admin*p=admin_head;
    char tmp_ID[100];
    char tmp_password[100];
    char tmp_name[100];
    //���ζ��� ID ���� ����
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
    printf("�ܼƶ��� %d λ������Ϣ\n",cnt);
    fclose(fp);
    return;
}

//���ļ��ж���ɼ���Ϣ
void read_score(FILE*fp){
    if(fp==NULL){
        printf("�ļ���ʧ��\n");
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
    //��ѧ����Ӧ�Ŀγ���Ŀ
    char tmp_course_count_c[100];
    int tmp_course_count;
    //�ɼ��ڵ��Ӧ������
    char tmp_score_c[100];
    double tmp_score;
    char tmp_subject_name[100];
    char tmp_credit_c[100];
    double tmp_credit;
    char tmp_is_required_course_c[100];
    int tmp_is_required_course;
    //���ζ��� ѧ��ID ѧ��רҵ �γ���Ŀ
    while(fgets(tmp_ID,sizeof(tmp_ID),fp)){
    	cnt_stu++;
        tmp_ID[strcspn(tmp_ID,"\n")]='\0';
        fgets(tmp_major,sizeof(tmp_major),fp);
        tmp_major[strcspn(tmp_major,"\n")]='\0';
        fgets(tmp_course_count_c,sizeof(tmp_course_count_c),fp);
        tmp_course_count_c[strcspn(tmp_course_count_c,"\n")]='\0';
        //���� ����tmp_majorѰ��רҵ
        q=major_head;
        while(q!=NULL){
            if(strcmp(q->name,tmp_major)==0)
                break;
            q=q->next;
        }
        if(q==NULL){
        	printf("�ڶ���� %d λѧ���ɼ�ʱ ���ִ���\n",cnt_stu);
            printf("�����ڴ�רҵ: %s ��ȡ����\n",tmp_major);
            continue;
        }
        //��� ����tmp_IDѰ�Ҷ�Ӧ��ѧ��
        o=q->student_linked_list;
        while(o!=NULL){
            if(strcmp(o->student_ID,tmp_ID)==0)
                break;
            o=o->next;
        }
        //��� �����ѧ����Ӧ�ĳɼ���Ϣ
        p=o->score_linked_list;
        //���ζ��� �ɼ� �γ����� ѧ�� �Ƿ�Ϊ���޿�
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
        //������һ��ѧ��֮ǰ �����ѧ����ǰ��GPA���
        o->GPA=calculate_GPA(o);
    }
    printf("�ܼƶ��� %d λѧ���ĳɼ�����\n",cnt_stu);
    printf("�ܼƶ��� %d �ſγ̵ĳɼ�����\n",cnt_cour);
    fclose(fp);
    return;
}

//ϵͳ��ʼ������ ���ȶ������е�ѧ�� ��ʦ�ͽ�����Ϣ
void system_initialization(){
    //����רҵ��Ϣ
    FILE*fp_major=fopen("major_data.txt","r");
    printf("���ڳ�ʼ��רҵ��Ϣ ���Ե�\n");
    read_major(fp_major);
    printf("-----------------------------------------------------\n");
    //����ѧ����Ϣ
    FILE*fp_student=fopen("student_data.txt","r");
    printf("���ڳ�ʼ��ѧ����Ϣ ���Ե�\n");
    read_student(fp_student);
    printf("-----------------------------------------------------\n");
    //�����ʦ��Ϣ
    FILE*fp_teacher=fopen("teacher_data.txt","r");
    printf("���ڳ�ʼ����ʦ��Ϣ ���Ե�\n");
    read_teacher(fp_teacher);
    printf("-----------------------------------------------------\n");
    //���������Ϣ
    FILE*fp_admin=fopen("admin_data.txt","r");
    printf("���ڳ�ʼ��������Ϣ ���Ե�\n");
    read_admin(fp_admin);
    printf("-----------------------------------------------------\n");
    //����ɼ���Ϣ
    FILE*fp_score=fopen("score_data.txt","r");
    printf("���ڳ�ʼ���ɼ���Ϣ ���Ե�\n");
    read_score(fp_score);
    printf("-----------------------------------------------------\n");
    printf("ϵͳ��ʼ���ɹ� ������ת���ִ�ѧ���ʼӷ���ɼ�����ϵͳ\n");
    printf("-----------------------------------------------------\n");
    system("pause"); 
    return;
}

//���������� ѡ��ѧ�� ��ʦ���߽�����е�¼
void screen(){
	system("cls");
    int choice=0;
	char choice_c[100];
    printf("��ӭ��¼���ִ�ѧ���ʼӷ���ɼ�����ϵͳ ��ѡ����\n");
    printf("*******************************\n");
    printf("* [1] ѧ����¼  [2] ��ʦ��¼  *\n");
    printf("* [3] �����¼  [4] �˳�ϵͳ  *\n");
    printf("*******************************\n");
    printf("��ѡ��:\n");
    gets(choice_c);
    choice=atoi(choice_c);
    if(choice==1)
        student_login();
    else if(choice==2)
        teacher_login();
    else if(choice==3)
        admin_login();
    else if(choice==4){
    	printf("ϵͳ�ɹ��ǳ� ��ӭ�´ε�¼\n");
        exit(0);
    }
    else{
        printf("��������ȷ��ѡ��\n");
        system("pause");
        system("cls");
        screen();
    }
}

//ѧ����¼�˺�������֤
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

//��ʦ��¼�˺�������֤
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

//�����¼�˺�������֤
struct admin*admin_login_verify(char*ID,char*password){
    struct admin*adm=admin_head;
    while(adm!=NULL){
        if(strcmp(ID,adm->admin_ID)==0 && strcmp(password,adm->password)==0)
            return adm;
        adm=adm->next; 
    }
    return NULL;
}

//ѧ���ϱ�ѧ������
void student_update_paper(){
    struct paper_node*p=paper_process_head;
    //pָ��paper_process_head�����һ���ڵ� 
    if(p!=NULL){
		while(p->next!=NULL)
	        p=p->next;
    }
    struct paper_node*r=(struct paper_node*)malloc(sizeof(struct paper_node));
    printf("������������Ŀ����������:\n");
    printf("ע�� ��ϵͳ�����һλ����ΪͨѶ����\n");
    char cnt_c[100];
    int cnt=0;
    while(1){
	    gets(cnt_c);
	    cnt=atoi(cnt_c);
	    if(cnt<=0 || cnt>=10)
	    	printf("������Ŀ�������� ����������\n");
	    else
	    	break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){
	    	printf("�������%d����:\n",i+1);
	        gets(r->author[i]);
	        if(strlen(r->author[i])==0)
	        	printf("���߲�����Ϊ�� ����������\n");
	        else 
	        	break;
    	}
    }
    for(int i=cnt;i<10;i++){
    	r->author[i][0]='\0';
	}
	while(1){
	    printf("��������������:\n");
	    gets(r->paper_name);
		if(strlen(r->paper_name)==0)
			printf("�������Ʋ�����Ϊ�� ����������\n");
		else
			break; 
	}
	while(1){
	    printf("�������ڿ����ƻ��������:\n");
	    gets(r->journal_or_conference_name);
	    if(strlen(r->journal_or_conference_name)==0)
	    	printf("�ڿ����ƻ�������Ʋ�����Ϊ�� ����������\n");
	    else
	    	break;
	}
	while(1){
	    printf("���������ķ���ʱ�� �������ո�ʽ����:\n");
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
	    	printf("���ķ���ʱ��������� ����������\n");
	}
	char range_c[100];
	while(1){
	    printf("���������ڻ����ĺŻ�ҳ��:\n");
	    gets(range_c);
	    r->range=atoi(range_c);
	    if(r->range>0)
	    	break;
	    else
	    	printf("����ڻ����ĺŻ�ҳ��������� ����������\n");
	}
	while(1){
	    printf("���������ļ���\n");
	    gets(r->level);
	    if(strlen(r->level)==0)
	    	printf("���ļ�������Ϊ�� ����������\n");
	    else
	    	break;
	}
	char GPA_add_c[100];
	while(1){
	    printf("��������Ԥ�ڵ����ʼӷ�:\n");
	    gets(GPA_add_c);
	    r->GPA_add=atof(GPA_add_c);
	    if(r->GPA_add<=0 || r->GPA_add>0.4)
	    	printf("�������ʼӷַǷ� ����������\n");
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
    printf("�ϱ��ɹ� �����ĵȴ��������\n");
    printf("�������������ϱ�����\n");
    system("pause");
    system("cls");
    student_update_GPA_add();
}

//ѧ���ϱ�����Ŀ
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
	    printf("�������Ա�������Ա����:\n");
	    gets(cnt_c);
	    cnt=atoi(cnt_c);
	    if(cnt<=0 || cnt>5){
	    	printf("��Ա����������� ����������\n"); 
		}
		else
			break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){
	    	printf("�������%dλ��Ա:\n",i+1);
	        gets(r->member[i]);
	        if(strlen(r->member[i])==0)
	        	printf("�󴴳�Ա������Ϊ�� ����������\n");
	        else 
	        	break;
    	}
    }
    for(int i=cnt;i<5;i++){
    	r->member[i][0]='\0';
	}
	while(1){
	    printf("������ָ����ʦ����:\n");
	    gets(r->instructor);
	    if(strlen(r->instructor)==0)
	    	printf("ָ����ʦ������Ϊ�� ����������\n");
	    else
	    	break;
	}
	while(1){
	    printf("��������Ŀ����:\n");
	    gets(r->project_name);
	    if(strlen(r->project_name)==0)
	    	printf("��Ŀ���Ʋ�����Ϊ�� ����������\n");
	    else
	    	break;
	}
	while(1){
	    printf("��������Ŀ���:\n");
		gets(r->code);
	    if(strlen(r->code)==0)
	    	printf("��Ŀ��Ų�����Ϊ�� ����������\n");
	    else
	    	break;
	}
	char year_c[100];
	char month_c[100];
	char day_c[100];
	while(1){ 
		while(1){
		    printf("�����������ʱ�� �������ո�ʽ����:\n");
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
				printf("����ʱ����Ч ����������\n");
			}
		}
		while(1){
		    printf("������󴴽���ʱ�� �������ո�ʽ����:\n");
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
				printf("����ʱ����Ч ����������\n");
			}
		}
		if(r->initiation_time.year>r->completion_time.year)
			printf("�������ʱ������ ����������\n");
		else if(r->initiation_time.year==r->completion_time.year && r->initiation_time.month>r->completion_time.month)
			printf("�������ʱ������ ����������\n");
		else if(r->initiation_time.year==r->completion_time.year && r->initiation_time.month==r->completion_time.month && r->initiation_time.day>r->completion_time.day)
			printf("�������ʱ������ ����������\n");
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
    printf("�ϱ��ɹ� �����ĵȴ��������\n");
    printf("�������������ϱ�����\n");
    system("pause"); 
    system("cls");
    student_update_GPA_add();
}

//ѧ���ϱ�������
void student_update_competition(){
	//�ҵ�����˵����һ���ڵ� 
    struct competition_node*p=competition_process_head;
    if(p!=NULL){
		while(p->next!=NULL)
	        p=p->next;
    }
    struct competition_node*r=(struct competition_node*)malloc(sizeof(struct competition_node));
    char cnt_c[100];
    int cnt;
	while(1){ 
	    printf("�������������񽱳�Ա����:\n");
		gets(cnt_c);
		cnt=atoi(cnt_c);
		if(cnt<=0 || cnt>10)
			printf("����������Ƿ� ����������\n");
		else
			break;
	}
    for(int i=0;i<cnt;i++){
    	while(1){ 
	    	printf("�������%dλ����:\n",i+1);
	        gets(r->awardee[i]);
	        if(strlen(r->awardee[i])==0)
	        	printf("���˲�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
    }
    for(int i=cnt;i<10;i++){
    	r->awardee[i][0]='\0';
	}
	while(1){
	    printf("�����뾺������:\n");
	    gets(r->competition_name);
	    if(strlen(r->competition_name)==0)
	    	printf("�������Ʋ�����Ϊ�� ����������\n");
	    else
	    	break;
	}
	while(1){
	    printf("�����뾺�����췽����:\n");
	    gets(r->organizer);
	    if(strlen(r->organizer)==0)
	    	printf("�������췽������Ϊ�� ����������\n");
		else
			break; 
	}
    char year_c[100];
	char month_c[100];
	char day_c[100];
    while(1){
		printf("�������ʱ�� �������ո�ʽ����:\n");
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
			printf("����ʱ����Ч ����������\n");
		}
	}
	while(1){
	    printf("������񽱵ȼ�:\n");
	    gets(r->award_level);
	    if(strlen(r->award_level)==0)
	    	printf("�񽱵ȼ�������Ϊ�� ����������\n");
	    else
	    	break;
	}
	char GPA_add_c[100];
	while(1){ 
	    printf("��������Ԥ�ڵ����ʼӷ�:\n");
	    gets(GPA_add_c);
	    r->GPA_add=atof(GPA_add_c);
	    if(r->GPA_add>0.4 || r->GPA_add<=0)
			printf("���ʼӷַǷ� ����������\n");
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
    printf("�ϱ��ɹ� �����ĵȴ��������\n");
    printf("�������������ϱ�����\n");
    system("pause");
    system("cls");
    student_update_GPA_add();
}

//ѧ���ϱ�ѧԺ���ʼӷ���� ������ ���ĺ;���
void student_update_GPA_add(){
    int choice;
    char choice_c[100];
    system("cls");
    printf("��ѡ���ϱ���Ŀ\n");
    printf("**************************************\n");
    printf("* [1] �ϱ�ѧ������  [2] �ϱ�����Ŀ *\n");
    printf("* [3] �ϱ�������  [4] �����ϼ�ѡ�� *\n");
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
        printf("��������ȷ��ѡ��\n");
        system("pause");
        system("cls");
        student_update_GPA_add();
    }
    return;
}

//��ʦ�ϴ��γ̳ɼ�
void teacher_course_upload(struct teacher*tea){
    struct major*q=major_head;
    while(q!=NULL){
        if(strcmp(q->name,tea->major)==0)
            break;
        q=q->next;
    }
    //��ʱ�����ж��Ƿ�Ϊ�� ��Ϊ�ض����ڴ�רҵ 
    struct student*o=q->student_linked_list;
    if(o==NULL){
    	printf("��רҵѧ��Ϊ�� �ϴ�ʧ��\n");
	    printf("���������ϲ�\n");
	    system("pause");
	    system("cls");
	    teacher_login();
	    return;
	}
	printf("��������ѧ���ɼ�:\n");
	printf("�γ�����:%s\n",tea->course); 
    printf("�γ�ѧ��:%2lf\n",tea->credit);
    printf("�γ�����:");
    if(tea->is_required_course){
    	printf("���޿�\n");
	}
	else{
		printf("ѡ�޿�\n");
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
	        printf("������ѧ��%s�ĳɼ�:\n",o->name);
	        gets(score_c);
	        r->score=atof(score_c);
	        if(r->score<=0 || r->score>100)
	        	printf("�ɼ��������� ����������\n");
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
    printf("�ϴ��ɹ�\n");
    printf("���������ϲ����\n");
    system("pause");
    system("cls");
    teacher_login();
    return;
}

//�������ѧ������
void admin_verify_paper(){
    struct paper_node*p=paper_process_head;
    printf("��ӭ��¼ѧ���������ϵͳ\n");
    if(p==NULL){
        printf("������Ҫ��˵�ѧ������\n");
        printf("���������ϼ�\n");
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
		printf("��ʱδ��˵�ѧ�������� %d ƪ\n",cnt);
		printf("������ʼ���\n"); 
	}
    //��˳����������ĩβ
    while(p!=NULL){
        printf("��������Ϊ:\n");
        for(int i=0;i<10;i++){
        	if(p->author[i][0]=='\0')
        		break;
        	printf("��Ա%d. %s\n",i+1,p->author[i]);
		}
        printf("��������Ϊ:%s\n",p->paper_name);
        printf("�ڿ����ƻ��������Ϊ:%s\n",p->journal_or_conference_name);
        printf("���ķ���ʱ��Ϊ:%d��%d��%d��\n",p->publication_time.year,p->publication_time.month,p->publication_time.day);
        printf("����ڻ����ĺŻ�ҳ��Ϊ:%d\n",p->range);
        printf("���ĵȼ�Ϊ:%s\n",p->level);
        printf("Ԥ�ڵ����ʼӷ�Ϊ:%.2lf\n",p->GPA_add);
        int choice;
        char choice_c[100];
        printf("��������˽��:\n");
	    printf("********************************\n");
	    printf("* [1] ���ͨ��  [2] ��˲�ͨ�� *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        //������ͨ�� ���뵽��������
        if(choice==1){
            //�����������Ϊ��
            if(paper_head==NULL){
                paper_head=p;
                p=p->next;
                paper_process_head=paper_process_head->next;
                paper_head->next=NULL;
            }
            //��������ǿ� ���뵽β�ڵ� 
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
            printf("�������� ����������\n");
            system("pause");
            system("cls");
        }
    }
    printf("��˳ɹ� ���������ϼ�\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//������˴���Ŀ
void admin_verify_innovate(){
    struct innovate_node*p=innovate_process_head;
    printf("��ӭ��¼����Ŀ���ϵͳ\n");
    if(p==NULL){
        printf("������Ҫ��˵Ĵ���Ŀ\n");
        printf("���������ϼ�\n");
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
		printf("��ʱδ��˵Ĵ���Ŀ�� %d ��\n",cnt);
		printf("������ʼ���\n");
	} 
    //��˳����������ĩβ
    while(p!=NULL){
        printf("����Ŀ����Ϊ:%s\n",p->project_name);
        printf("ָ����ʦ����Ϊ:%s\n",p->instructor);
        printf("��Ŀ���Ϊ:%s\n",p->code);
        printf("������ʱ��Ϊ:%d��%d��%d��\n",p->initiation_time.year,p->initiation_time.month,p->initiation_time.day);
        printf("�󴴽���ʱ��Ϊ:%d��%d��%d��\n",p->completion_time.year,p->completion_time.month,p->completion_time.day);
        printf("�󴴳�ԱΪ:\n");
        for(int i=0;i<5;i++){
        	if(p->member[i][0]=='\0')
        		break;
        	printf("��Ա%d. %s\n",i+1,p->member[i]);
		}
        int choice;
        char choice_c[100]; 
        printf("��������˽��\n");
	    printf("********************************\n");
	    printf("* [1] ���ͨ��  [2] ��˲�ͨ�� *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
        	p->GPA_add=0.1;
            //���������Ϊ��
            if(innovate_head==NULL){
                innovate_head=p;
				p=p->next;
                innovate_process_head=innovate_process_head->next;
                innovate_head->next=NULL;
            }
            //������ǿ�
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
            printf("�������� ����������\n");
        	system("pause");
            system("cls");
        }
    }
    printf("��˳ɹ� ���������ϼ�\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//������˾�����
void admin_verify_competition(){
    struct competition_node*p=competition_process_head;
    printf("��ӭ��¼���������ϵͳ\n");
    if(p==NULL){
        printf("������Ҫ��˵ľ�����\n");
        printf("���������ϼ�\n");
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
		printf("��ʱδ��˵ľ������� %d ��\n",cnt);
		printf("������ʼ���\n");
	}
    //��˳����������ĩβ
    while(p!=NULL){
        printf("��������Ϊ:%s\n",p->competition_name);
        printf("�������췽Ϊ:%s\n",p->organizer);
        printf("�񽱳�ԱΪ:%s\n",p->awardee);
        for(int i=0;i<10;i++){
        	if(p->awardee[i][0]=='\0')
        		break;
        	printf("��Ա%d. %s\n",i+1,p->awardee[i]);
		}
        printf("��ʱ��Ϊ:%d��%d��%d��\n",p->award_time.year,p->award_time.month,p->award_time.day);
        printf("�񽱵ȼ�Ϊ:\n");
        printf("Ԥ�ڵ����ʼӷ�Ϊ:%.2lf\n",p->GPA_add);
        int choice;
        char choice_c[100]; 
        printf("��������˽��\n");
	    printf("********************************\n");
	    printf("* [1] ���ͨ��  [2] ��˲�ͨ�� *\n");
	    printf("********************************\n");
        gets(choice_c);
        choice=atoi(choice_c);
        if(choice==1){
            //�����������Ϊ��
            if(competition_head==NULL){
                competition_head=p;
                p=p->next;
                competition_process_head=competition_process_head->next;
            	competition_head->next=NULL;
            }
            //��������ǿ�
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
            printf("�������� ����������\n");
            system("pause");
            system("cls");
        }
    }
    printf("��˳ɹ� ���������ϼ�\n");
    system("pause");
    system("cls");
    admin_verify_GPA_add();
    return;
}

//����������ʼӷ����
void admin_verify_GPA_add(){
    int choice;
    char choice_c[100]; 
    system("cls");
    printf("��ӭ�������ʼӷ����ϵͳ\n");
    printf("��ѡ�������Ŀ\n");
    printf("**************************************\n");
    printf("* [1] ���ѧ������  [2] ��˴���Ŀ *\n");
    printf("* [3] ��˾�����  [4] �����ϼ�ѡ�� *\n");
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
        printf("��������ȷ��ѡ��\n");
        system("pause");
        system("cls");
        admin_verify_GPA_add();
    }
}

//����תרҵѧ���춯���� �������ѧ�� ����ɾ��ѧ��
struct student* Find_change_stu(struct major* head ,char origin_major[100], char change_stu_ID[100])
{
    if(head == NULL)
    {
        printf("רҵΪ�� ��������\n");
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
        printf("δ�ҵ� %s רҵ ��������\n", origin_major);
        return NULL;
    }  
    else
    {
        struct student* find_stu = curr_major->student_linked_list;
        struct student* prev_stu = NULL;
        if(find_stu == NULL)
        {
            printf("ԭרҵѧ����ϢΪ�� ��������\n");
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
            printf("δ�ҵ�ѧ��Ϊ %s ��ѧ�� ��������\n", change_stu_ID);
            return NULL;
        }  
        else
        {
            if(prev_stu == NULL)  //  ͷ
            {
                curr_major->student_linked_list = find_stu->next;
            }
            else if(find_stu->next != NULL)    // �м�
            {
                prev_stu->next = find_stu->next;
            }
            else   // β
            {
                prev_stu->next = NULL;
            }
            find_stu->next = NULL;  // �Ͽ���һ��!!!
            return find_stu; 
        }  
    }
}

//תרҵѧ������
void change_stu_major(struct major* head ,char origin_major[100], char new_major[100], char change_stu_ID[100])
{
    struct student* change_stu = Find_change_stu(head ,origin_major, change_stu_ID);
    if(change_stu == NULL)
    {
        printf("����ѧ��Ϊ %s ��תרҵѧ��ʧ��\n", change_stu_ID);
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
			printf("δ�ҵ� %s רҵ, ����ѧ��Ϊ %s ��תרҵѧ��ʧ��!\n", new_major, change_stu_ID);
			return;
		}  
        else
        {	
        	struct student* old = curr_major->student_linked_list;
        	
    		printf("%s רҵԭ��ѧ��Ϊ:\n", curr_major);
    		while(old != NULL)
    		{
				printf("%s\n", old->name);
				old = old->next;
			}
            struct student* find_pos = curr_major->student_linked_list;   // ���������λ��
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
	        printf("Ŀǰ ѧ�� %s ���� %s ͬѧ ��ת�� %s רҵ\n", change_stu->student_ID,change_stu->name,change_stu->major); 
	        printf("\n");
	    	printf("%s רҵĿǰѧ��Ϊ:\n", new_major);
	    	struct student* res = curr_major->student_linked_list;
	    	while(res != NULL)
	    	{
				printf("%s\n", res->name);
				res = res->next;
			}
        }
	}  
}

//�������תרҵ����
void admin_major_change(struct major* head)
{
	printf("��ӭ����תרҵϵͳ!\n");
	char origin_major[100];
	while(1){
	    printf("������תרҵѧ����ԭרҵ����:\n");
	    gets(origin_major);
	    if(strlen(origin_major)==0)
	    	printf("ԭרҵ������Ϊ�� ����������\n");
	    else
	    	break;
	}
	char new_major[100];
	while(1){
	    printf("������תרҵѧ������רҵ����:\n");
	    gets(new_major);
	    if(strlen(new_major)==0)
	    	printf("��רҵ������Ϊ�� ����������\n");
	    else
	    	break;
	}
	char change_stu_ID[100];
	while(1){ 
	    printf("������תרҵѧ����ѧ��:\n");
	    gets(change_stu_ID);
	    if(strlen(change_stu_ID)==0)
	    	printf("תרҵѧ��ѧ�Ų�����Ϊ�� ����������\n");
	    else
	    	break;
	}
    change_stu_major(head, origin_major, new_major, change_stu_ID);	
    printf("�������� ����������һ��\n");
    system("pause");
    system("cls");
    admin_login();
}

//��������������
void InsertList()
{
    struct paper_node* new_node = (struct paper_node*)malloc(sizeof(struct paper_node));
    char author_num_string[100];
    int author_num_string_res=0;
	while(1)
    {
        printf("������Ҫ��ӵ�������������(������ʮ����):\n");
        printf("ע�� ��ϵͳ�����һ������ΪͨѶ����\n"); 
        gets(author_num_string);
        author_num_string_res = atoi(author_num_string);
        if(author_num_string_res > 10)
            printf("�����������������������10 ����������\n");
        else if(author_num_string_res <= 0)
        	printf("������������������Ϸ� ����������\n");
        else
        {
            printf("�밴˳��������������(�Իس��ָ�):\n");
            for(int i = 0; i < author_num_string_res; i++)
            {
            	while(1){
                	gets(new_node->author[i]);
                	if(strlen(new_node->author[i])==0)
						printf("���߲�����Ϊ�� ����������\n");
					else
						break; 
            	}
            }
            for(int i=author_num_string_res;i<10;i++){
            	new_node->author[i][0] = '\0';
			}
            printf("������Ľ������:\n");
            for(int i = 0; i < author_num_string_res; i++)
            {
                printf("���ĵ� %d ����Ϊ: %s \n", i+1, new_node->author[i]);
            }
            break;
        }
    }
    char buffer[1000]; 
    while (1) 
    {
        printf("������Ҫ��ӵ���������:\n");
        gets(buffer);
		if(strlen(buffer)!=0)
        	break;
        else
            printf("������� ����������\n");
    }
    strcpy(new_node->paper_name, buffer);
    printf("����������������ǣ�%s\n", new_node->paper_name);
    // ���� �����ڿ�/��������
    while(1){
	    printf("������Ҫ��ӵ����������ڿ�/��������:\n");
	    gets(new_node->journal_or_conference_name);
	    if(strlen(new_node->journal_or_conference_name)==0)
	    	printf("�ڿ���������Ʋ�����Ϊ�� ����������\n");
	    else
	    	break;
	}
    printf("����������������ڿ�/���������ǣ�%s\n", new_node->journal_or_conference_name);
    char year_c[100];
	char month_c[100];
	char day_c[100];
	while(1)
    {
    	// ���� ���ķ���ʱ��
	    printf("������Ҫ��ӵ����ķ���ʱ��(���� �� �� �� ��˳������ ���Իس��ָ�):\n");
	    gets(year_c);
	    new_node->publication_time.year=atoi(year_c);
	    gets(month_c);
	    new_node->publication_time.month=atoi(month_c);
	    gets(day_c);
	    new_node->publication_time.day=atoi(day_c);
	    if(check_time(new_node->publication_time)==0)
	    	printf("����������ڲ��Ϸ� ����������\n");
	    else
	    {
	    	printf("����������ķ���ʱ����: %d �� %d �� %d ��\n", new_node->publication_time.year, new_node->publication_time.month, new_node->publication_time.day);
	    	break;
		}
	}
    // ���� ���ľ���ڻ����ĺŻ�ҳ��
    char range_c[100];
	while(1){
	    printf("������Ҫ��ӵ����ľ���ڻ����ĺŻ�ҳ��:\n");
	    gets(range_c);
	    new_node->range=atoi(range_c);
	    if(new_node->range<=0)
	    	printf("���ľ���ڻ����ĺŻ�ҳ��������Ч ����������\n");
	    else
	    	break;
	}
    printf("����������ľ���ڻ����ĺŻ�ҳ����: %d\n", new_node->range);
    // ���� ���Ķ�Ӧ�ļ���
    while(1){
	    printf("������Ҫ��ӵ����ļ���:\n");
		gets(new_node->level);
		if(strlen(new_node->level)==0)
			printf("���ļ�������Ϊ�� ����������\n");
		else
			break;
	}
    printf("����������ļ����ǣ�%s\n", new_node->level);
	char GPA_add_c[100];
	while(1)
	{
	    printf("������Ҫ��ӵ����Ķ�Ӧ�����ʼӷ�:\n");
	    gets(GPA_add_c);
	    new_node->GPA_add=atof(GPA_add_c);
	    if(new_node->GPA_add == 0) printf("����������ʼӷֲ��Ϸ� ����������\n");
	    else if(new_node->GPA_add > 0.4)
	    {
	    	printf("����������Ķ�Ӧ�����ʼӷ� %.2lf ���� ���ʼӷ����� 0.4\n", new_node->GPA_add);
	    	printf("����������\n");
		}
	    else
		{
			printf("����������Ķ�Ӧ�����ʼӷ���: %.2lf\n", new_node->GPA_add);
			break;
		} 
	}
    if(paper_head == NULL)
    {
        paper_head = new_node;
        paper_head->next = NULL;
    	printf("�����ɹ�! ����������һ��\n");
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
		// ���½ڵ���ӵ������β��
        temp->next = new_node;
        new_node->next = NULL;
	    printf("�����ɹ�! ����������һ��\n");
		system("pause");
		system("cls");
		admin_check_paper();
	    return;
	} 
}

//��������ɾ������
void DeleteList(char paper[1000])
{
    if(paper_head == NULL)
    {
        printf("��ǰ������Ϣ���Ϊ��!\n");
        printf("����������һ��\n");
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
            printf("ɾ���ɹ� ����������һ��\n");
        	system("pause");
    		system("cls");
    		admin_check_paper();
            return;
        }
        else{
            printf("��Ǹ! δ�ҵ�����%s����Ϣ\n", paper);
            printf("����������һ��\n");
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
            printf("ɾ���ɹ�������������һ��\n");
        	system("pause");
    		system("cls");
    		admin_check_paper();
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    // last ���һ�����
    if(strcmp(paper, curr->paper_name) == 0)
    {
        free(curr);
        curr = NULL;
        prev->next=NULL;
        printf("ɾ���ɹ� ����������һ��\n");
    	system("pause");
		system("cls");
		admin_check_paper();
    }
    else
    {
        printf("��Ǹ δ�ҵ����� %s ����Ϣ", paper);
        printf("����������һ��\n");
    	system("pause");
		system("cls");
		admin_check_paper();
        return;
    }
}

//�������Ķ�Ӧָ�� �����
struct paper_node* Find(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("��ǰ������Ϣ��Ϊ��\n");
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

//�鿴������Ϣ
void CheckList(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("��ǰ������Ϣ��Ϊ��\n");
        printf("����������һ��\n"); 
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
            printf("��������Ϊ: %s\n", paper);
            printf("��������Ϊ: \n");
            for (int i = 0; i < 10; i++) 
			{
                if (curr->author[i][0] == '\0')
                    break;
                printf("�� %d ����: %s\n", i + 1, curr->author[i]);
            }
            printf("���������ڿ����ƻ��������Ϊ: %s\n", curr->journal_or_conference_name);
            printf("�������ķ�������Ϊ: %d�� %d�� %d��\n", curr->publication_time.year, curr->publication_time.month, curr->publication_time.day);
            printf("���ľ���ڻ����ĺŻ�ҳ��Ϊ: %d\n", curr->range);
            printf("���ļ���Ϊ: %s\n", curr->level);
            printf("�������ʼӷ�Ϊ: %.2lf\n", curr->GPA_add);
	        printf("����������һ��\n"); 
	        system("pause");
	    	system("cls");
	    	admin_check_paper();
			return;
        }
        curr = curr->next;
    }
    printf("��Ǹ δ�ҵ����� %s ����Ϣ\n", paper);
    printf("����������һ��\n");
    system("pause");
    system("cls");
    admin_check_paper();
}

//���ĵĸ��²���
void UpdateList(struct paper_node* head, char paper[1000])
{
    if(head == NULL)
    {
        printf("���� %s �ӷ����Ϊ�� �޿��޸��ļ�\n", paper);
        printf("����������һ��\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
    struct paper_node* res = Find(head, paper);
    if(res == NULL)
	{
		printf("��Ǹ δ�ҵ����� %s ����Ϣ\n", paper);
		printf("����������һ��\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
	}  
    else
    {
        printf("��������Ҫ�޸ĵİ��:\n");
        int select = 1;
        while(select)
        {
            printf("***********************************\n");
            printf("* [1] ��������  [2] ��������  *\n");
            printf("* [3] �ڿ�����  [3] ��������  *\n");
            printf("* [4] ����ʱ��  [5] ����ҳ��  *\n");
            printf("* [6] ���ļ���  [7] ���ʼӷ�  *\n");
            printf("* [8] �����޸�  [*] ********  *\n");
            printf("***********************************\n");
            printf("��ѡ��:\n");
            char select_c[100];
            gets(select_c);
            select = atoi(select_c);
            if(select == 0)
            {
            	printf("��������Ƿ� ��������\n"); 
            	continue;
			}
            switch(select)
            {
            case 1:
                printf("������Ҫ�޸ĵ����� ��1��ʼ����:\n");
                int num = 0;
        		char num_c[100]; 
        		gets(num_c);
        		num=atoi(num_c);
        		if(num == 0)
				{
					printf("��������Ƿ� ����������\n");
					continue;
				}
				while(1){ 
	                printf("������Ҫ���������������: \n");
	                gets(res->author[num-1]);
	                if(strlen(res->author[num-1])==0)
	                	printf("�������Ʋ�����Ϊ�� ����������\n");
	                else
	                	break;
            	}
                printf("���� %s �� %d ���߱�����Ϊ: %s\n", paper, num, res->author[num-1]);
                break;
            case 2:
            	while(1){
	                printf("��������������������:\n");
	                gets(res->paper_name);
	                if(strlen(res->paper_name)==0)
	                	printf("�������Ʋ�����Ϊ�� ����������\n");
	                else
	                	break;
            	}
                printf("���� %s ���Ʊ�����Ϊ: %s\n", paper, res->paper_name);
                break;
            case 3:
            	while(1){
	                printf("�������������ڿ�/��������:\n");
	                gets(res->journal_or_conference_name);
	                if(strlen(res->journal_or_conference_name)==0)
	                	printf("�ڿ���������Ʋ�����Ϊ�� ����������\n");
	                else
	                	break;
            	}
                printf("\n���� %s �ڿ�/�������Ʊ�����Ϊ: %s\n", paper, res->journal_or_conference_name);
                break;
            case 4:
				while(1){
	                char year_c[100];
					char month_c[100];
					char day_c[100];
					printf("���������������ķ���ʱ��(���� �� �� �� ��˳������):\n");
				    gets(year_c);
				    res->publication_time.year=atoi(year_c);
				    gets(month_c);
				    res->publication_time.month=atoi(month_c);
				    gets(day_c);
				    res->publication_time.day=atoi(day_c);
				    if(check_time(res->publication_time))
				    	break;
				    else
				    	printf("���ķ���ʱ��Ƿ� ����������\n");
				}
                printf("���� %s �������ڱ�����Ϊ: %d�� %d�� %d��\n", paper, res->publication_time.year, res->publication_time.month, res->publication_time.day);
                break;
            case 5:
                
                while(1){
					char page_c[100];
					printf("����������������ҳ��:\n");
	        		gets(page_c);
	        		res->range=atoi(page_c);
	        		if(res->range<=0)
	        			printf("����ҳ��Ƿ� ����������\n");
	        		else
						break; 
        		}
                printf("���� %s ҳ�뱻����Ϊ: %d\n", paper, res->range);
                break;
            case 6:
            	while(1){
	                printf("���������������ļ���:>\n");
	                gets(res->level);
	                if(strlen(res->level)==0)
	                	printf("���ļ�������Ϊ�� ����������\n");
	                else
	                	break;
            	}
                printf("���� %s ���𱻸���Ϊ: %s\n", paper, res->level);
                break;
            case 7:
                
                while(1){
					char gpa_c[100];
					printf("�������������������ʼӷ�:\n");
	        		gets(gpa_c);
	        		res->GPA_add=atof(gpa_c);
	        		if(res->GPA_add<=0 || res->GPA_add>0.4)
	        			printf("�������ʼӷַǷ� ����������\n");
	        		else
	        			break;
        		}
                printf("���� %s ���ʼӷֱ�����Ϊ: %lf\n", paper, res->GPA_add);
                break;
            case 8:
            	select = 0;
                break;
            default:
                printf("������������ ����������\n");
                break;
            }
        }
        printf("�����޸Ľ��� ף��������� �ټ�\n");
        printf("����������һ��\n");
        system("pause");
    	system("cls");
    	admin_check_paper();
        return;
    }
}

//qsort�ȽϺ��� ����ѧҵ���㽵������
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

//qsort�ȽϺ��� �����ۺϼ��㽵������
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

// �ȽϺ������Ȱ���GPA�������У����GPA��ͬ����ĳ�ſγ̳ɼ���������
int compareTotalGPA_Plus(const void *a, const void *b) 
{
	struct student *p1 = *(struct student **)a;
  	struct student *p2 = *(struct student **)b;

    // ���GPA��ͬ��ֱ�ӱȽ�GPA
    if (p1->GPA + p1->GPA_add != p2->GPA + p2->GPA_add) {
        return (p2->GPA + p2->GPA_add - p1->GPA + p1->GPA_add) > 0 ? 1 : -1;
    }
    // ���GPA��ͬ����Ƚ�ĳ�ſγ̳ɼ�
    else {
        return (p2->GPA_add - p1->GPA_add);
    }
}

//qsort�ȽϺ��� ����ƽ���ɼ���������
int compareAvgScore(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->AvgScore > p2->AvgScore)   
    return -1;       // qsortĬ����������  �� С�� �� ���� -1  ����������Ҫ���� ��Ӧ�÷�����
  else if(p1->AvgScore < p2->AvgScore)
    return 1;
  else
    return 0;
}

//qsort��ĳһ�ſγɼ����бȽ�
int compareProgram(const void *a, const void *b)
{
  struct student *p1 = *(struct student **)a;
  struct student *p2 = *(struct student **)b;
  if(p1->score_linked_list->score > p2->score_linked_list->score)   // ������ܻ������� ���ܱȽϵ��ǳɼ�����ĵ�һ�ſγ�
    return -1;       // qsortĬ����������  �� С�� �� ���� -1  ����������Ҫ���� ��Ӧ�÷�����
  else if(p1->score_linked_list->score < p2->score_linked_list->score)
    return 1;
  else
    return 0;
}

//��������ĳ���
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

//�鲢����ȽϺ��� ����GPA����
int MergeCompareAcademicGPA(struct student *a, struct student *b) {
    if (a->GPA < b->GPA )
        return -1;
    else if (a->GPA > b->GPA )
        return 1;
    else
        return 0;
}

//�鲢���� ����GPA+���ʼӷ�����
int MergeCompareTotalGPA(struct student *a, struct student *b) {
    if (a->GPA + a->GPA_add < b->GPA + b->GPA_add)
        return -1;
    else if (a->GPA + a->GPA_add > b->GPA + b->GPA_add)
        return 1;
    else
        return 0;
}

int MergeCompareTotalGPA_Plus(struct student *a, struct student *b) {
    // ���GPA��ͬ��ֱ�ӱȽ�GPA
    if (a->GPA + a->GPA_add != b->GPA + b->GPA_add) 
	{
        return (b->GPA + b->GPA_add - a->GPA + a->GPA_add) > 0 ? -1 : 1;     // �� qsort��һ�� 
    }
    // ���GPA��ͬ����Ƚ�ĳ�ſγ̳ɼ�
    else {
        return (b->GPA_add - a->GPA_add);
    }
}

//�鲢����
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

//merge�鲢 ����������ͷ�ڵ�
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

//������Ĺ鲢����
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

//�ۺϼ������� �鲢
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
	fprintf(fp,"����\t\tѧ��\t\t\t����\t\tѧҵ����\t\t��������Ŀ�ӷ�\t�ۺϼ���\t\tרҵ\n");
    for(int i = 1; i <= num; i++)
    {
    	if(strlen(p->name)==6)
        	fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i, p->student_ID, p->name, p->GPA, p->GPA_add, p->GPA+p->GPA_add, p->major);
        else
        	fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i, p->student_ID, p->name, p->GPA, p->GPA_add, p->GPA+p->GPA_add, p->major);
        p = p->next;
    }
	fclose(fp);
	printf("�ۺϼ������������ѷ���\n");
}

//����ָ��ѧ�� ������GPA_add 
double cal_GPA_add(struct student*tmp){
	double sum_GPA_add=0;
	//���� ֻ�������һ����
	struct paper_node*tmp_paper=paper_head; 
	while(tmp_paper!=NULL){
		if(strcmp(tmp_paper->author[0],tmp->name)==0)
			sum_GPA_add+=tmp_paper->GPA_add;
		tmp_paper=tmp_paper->next;
	}
	//�� ��һ������0.1 �ڶ�������0.05 
	struct innovate_node*tmp_innovate=innovate_head;
	while(tmp_innovate!=NULL){
		if(strcmp(tmp_innovate->member[0],tmp->name)==0)
			sum_GPA_add+=0.1;
		else if(strcmp(tmp_innovate->member[1],tmp->name)==0)
			sum_GPA_add+=0.05;
		tmp_paper=tmp_paper->next;
	}
	//���� ֻ��ǰ������ 
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

//ѧҵ�������� qsort
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
	fprintf(fp,"����\t\tѧ��\t\t\t����\t\tѧҵ����\t\t��������Ŀ�ӷ�\t�ۺϼ���\t\tרҵ\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	printf("ѧҵ���������ѷ���\n");
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
	fprintf(fp,"����\t\tѧ��\t\t\t����\t\tѧҵ����\t\t��������Ŀ�ӷ�\t�ۺϼ���\t\tרҵ\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->GPA, list[i]->GPA_add, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	printf("�ۺϼ��������ѷ���\n");
    free(list);
}

//�γ����� qsort
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
	fprintf(fp,"�γ�: %s\n", list[0]->score_linked_list->subject_name);
	fprintf(fp,"����\t\tѧ��\t\t\t����\t\t�γ̳ɼ�\tѧҵ����\t\t�ۺϼ���\t\tרҵ\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.1lf\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name,list[i]->score_linked_list->score, list[i]->GPA,  list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.1lf\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name,list[i]->score_linked_list->score, list[i]->GPA,  list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}

	fclose(fp);
	
	printf("�γ������ѷ���\n");

    free(list);
}

//����ָ��ѧ�� ������ ƽ��ѧҵ�ɼ� 
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

//�ۺϼ������� qsort
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
	fprintf(fp,"����\t\tѧ��\t\t\t����\t\tƽ���ɼ�\t\t�ۺϼ���\t\tרҵ\n");
    for(int i = 0; i < num; i++)
    {
    	if(strlen(list[i]->name)==6)
			fprintf(fp,"%d\t\t%s\t%s\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->AvgScore, list[i]->GPA+list[i]->GPA_add, list[i]->major);
		else
			fprintf(fp,"%d\t\t%s\t%s\t\t%.4lf\t\t%.4lf\t\t%s\n", i+1, list[i]->student_ID, list[i]->name, list[i]->AvgScore, list[i]->GPA+list[i]->GPA_add, list[i]->major);
	}
	fclose(fp);
	
	printf("ƽ���ɼ������ѷ���\n");

    free(list);
}

//�����������ɼ�����������
void admin_calculate_GPA_added(struct major* major_head)
{
    if(major_head == NULL)
    {
        printf("����רҵΪ�� ������������ʧ��\n");
        printf("���������ϼ�\n");
        system("pause");
        system("cls");
        admin_login();
        return;
    }
	char checkMajor[100];
	while(1){
	    printf("������Ҫ��ѯרҵ������רҵ����:\n"); 
	    gets(checkMajor);
	    if(strlen(checkMajor)==0)
	    	printf("רҵ���Ʋ�����Ϊ�� ����������\n");
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
        printf("δ�ҵ� %s רҵ��Ϣ ������������ʧ��\n", checkMajor);
        printf("���������ϼ�\n");
        system("pause");
        system("cls");
        admin_login(); 
        return;
    }

	if(major_head->is_published == 0)
    {
        printf(" %s רҵ��������δ���� �鿴��������ʧ��\n", major_head->name);
        printf("��ѡ���Ƿ񷢲���������: (1)�� (2)�� \n"); 
		int choice = 0;
		char choice_c[100]; 
		gets(choice_c);
		choice=atoi(choice_c);
		if(choice == 1) 
		{
			printf("���������ϼ�\n");
	        system("pause");
	        system("cls");
	        admin_login(); 
			return;
		}
		else if(choice == 2)
		{
			printf("���ڼ������������� �����ĵȴ�\n");
			printf("\n");
		}
		else{
			printf("������Чѡ�� ����������\n");
			system("pause");
			system("cls");
			admin_calculate_GPA_added(major_head);
			return;
		}
		
    }
    else if(clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s רҵ���������ѹ��� �鿴��������ʧ��\n", major_head->name);
        printf("��ѡ���Ƿ񷢲���������: (1)�� (2)�� \n"); 
        major_head->is_published = 0;
		int choice = 0;
		char choice_c[100]; 
		gets(choice_c);
		choice=atoi(choice_c);
		if(choice == 1) 
		{
			printf("���������ϼ�\n");
	        system("pause");
	        system("cls");
	        admin_login();
			return;
		}
		else if(choice == 2)
		{
			printf("���ڼ������������� �����ĵȴ�\n");
			printf("\n");
		}
		else{
			printf("������Чѡ�� ����������\n");
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
    printf("\nרҵ���������ѷ��� ��Чʱ��1���� ��鿴����\n");
	
    printf("���������ϼ�\n");
    system("pause");
    system("cls");
    admin_login(); 
    return;
}

//ѧ����ѯ����������� ����Ŀǰ�Ƿ񷢲����д���
void student_find_rank_M(struct major* major_head, char*name)
{  
    if(major_head->is_published == 0)
    {
        printf(" %s רҵ����δ���� �鿴��������ʧ��", major_head->name);
        return;
    }
    else if(clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s רҵ����������ʾ�ѹ��� �鿴��������ʧ��", major_head->name);
        return;
    } 
    struct student* head = major_head->student_linked_list;
    if(head == NULL) 
    {
        printf("��ǰרҵ����Ϊ�� ��ѯʧ��\n");
        return;
    } 
    int cnt = 0;
    if(major_head->is_published == 1 && clock()-major_head->publish_time > 60*(double)CLOCKS_PER_SEC)   //רҵ������������ �� ����δ����
    {
        int flag = 0;// ��־�Ƿ��ҵ���ѧ��
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
            printf("����\t\tѧ��\t\t\t����\t\tѧҵ����\t\t��������Ŀ�ӷ�\t�ۺϼ���\t\tרҵ\n"); 
	    	if(strlen(head->name)==6)
				printf("%d\t\t%s\t%s\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", cnt, head->student_ID, head->name, head->GPA, head->GPA_add, head->GPA+head->GPA_add, head->major);
			else
				printf("%d\t\t%s\t%s\t\t%.4lf\t\t%.2lf\t\t\t\t%.4lf\t\t%s\n", cnt, head->student_ID, head->name, head->GPA, head->GPA_add, head->GPA+head->GPA_add, head->major);
        }
        else    printf("δ�ҵ���ѧ����Ϣ!");
    }
}

//ѧ����ѯ �������� qsort
void student_find_rank(struct major* major_head, char*name)
{
    if(major_head->is_published == 0)// רҵ����δ����
    {
        printf(" %s רҵ����δ���� �鿴��������ʧ��\n", major_head->name);
        printf("���������ϲ�\n");
		system("pause");
		student_login();
		return;
    }
	// int publish_time;רҵ�������� ʣ�� ��ʾʱ��
    else if(clock()-major_head->publish_time>60*(double)CLOCKS_PER_SEC)
    {
        printf(" %s רҵ����������ʾ�ѹ��� �鿴��������ʧ��\n", major_head->name);
        printf("���������ϲ�\n");
		system("pause");
		student_login();
		return;
    } 
    struct student* head = major_head->student_linked_list;
    if(head == NULL) 
    {
        printf("��ǰרҵ����Ϊ�� ��ѯʧ��\n");
        printf("���������ϲ�\n");
		system("pause");
		student_login();
		return;
    }
    int num = getLength(head);
	//רҵ������������
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
        printf("����\tѧ��\t\t����\tѧҵ����\t��������Ŀ�ӷ�\t\t�ۺϼ���\tרҵ\n");
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
        else printf("δ�ҵ���ѧ����Ϣ!\n");
        free(list);
    }
    printf("���������ϲ�\n");
	system("pause");
	student_login();
	return;
}

// �ھ���������¼���µľ���
void addCompetition(char competition_name[], char organizer[], char award_level[], int num,char awardee[][100], struct time award_time) {
    // �����µľ����ڵ�
    struct competition_node* new_competition = (struct competition_node*)malloc(sizeof(struct competition_node));
    if (new_competition == NULL) {
        printf("�ڴ����ʧ�� �޷���Ӿ���\n");
        return;
    }
    // �����¾����ڵ�Ĳ���
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
    // �����������Ϊ�գ����½ڵ�����Ϊͷ�ڵ�
    if (competition_head == NULL) {
        competition_head = new_competition;
        printf("���� %s ��ӳɹ�\n", competition_name);
        return;
    }

    // ���� ������ĩβ�����µľ����ڵ�
    struct competition_node* temp = competition_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_competition;

    printf("���� %s ��ӳɹ�\n", competition_name);
}

//�Ƚ�������ά�����Ƿ����
int compareAwardees(char arr1[][100], char arr2[][100], int size) {
    for (int i = 0; i < size; ++i) {
        if (strcmp(arr1[i], arr2[i]) != 0) {
            return 0;
        }
    }
    return 1;
}

// ɾ�����������еľ���
//���awardee_count������
void deleteCompetition( char competition_name[], char awardee[][100], int AWARDEE_COUNT) {
    struct competition_node* temp = competition_head;
    struct competition_node* prev = NULL;

    // ���Ҫɾ���ľ�����ͷ�ڵ�
    if (temp != NULL && strcmp(temp->competition_name, competition_name) == 0 && compareAwardees(temp->awardee, awardee, AWARDEE_COUNT)) {
        competition_head = temp->next;
        free(temp);
        printf("���� %s ɾ���ɹ�\n", competition_name);
        return;
    }

    // �ҵ�Ҫɾ���ľ���
    while (temp != NULL && (strcmp(temp->competition_name, competition_name) != 0 || !compareAwardees(temp->awardee, awardee, AWARDEE_COUNT))) {
        prev = temp;
        temp = temp->next;
    }

    // ���δ�ҵ�����
    if (temp == NULL) {
        printf("δ�ҵ�ѧ�� %s ����Ӧ�ľ��� %s\n", awardee, competition_name);
        return;
    }

    // ��������ɾ������
    prev->next = temp->next;
    free(temp);
    printf("���� %s ɾ���ɹ�\n", competition_name);
}

// �޸ľ��������еľ����񽱵ȼ�
//ָ��������ͷ����ָ�� head��Ҫ�޸ĵľ������� competition_name���Լ��µĽ���ȼ� new_award_level
void modifyCompetition(char competition_name[], char awardee[][100],int num, char new_award_level[]) {
    struct competition_node* temp = competition_head;
    // �ҵ�Ҫ�޸ĵľ���
    while (temp != NULL) {
    	if(strcmp(temp->competition_name, competition_name)==0 && compareAwardees(temp->awardee, awardee, num)){
			break; 
		}
        temp = temp->next;
    }
    // ���δ�ҵ�����
    if (temp == NULL) {
        printf("δ�ҵ����� %s �޸�ʧ��\n", competition_name);
        return;
    }
    // �޸ľ����Ľ���ȼ�
    strcpy(temp->award_level, new_award_level);
    printf("���� %s �Ľ���ȼ��޸ĳɹ�\n", competition_name);
}


//��ѯ���������еľ���������
void queryCompetition(char competition_name[]) {
    struct competition_node* temp = competition_head;
    int found = 0; // ��־�Ƿ��ҵ�����
    // ����������������ָ�����Ƶľ���
    while (temp != NULL) {
        if (strcmp(temp->competition_name, competition_name) == 0) {
            found = 1;
            // ��ʾ��������ϸ��Ϣ
            printf("��������: %s\n", temp->competition_name);
            printf("���췽: %s\n", temp->organizer);
            printf("����ȼ�: %s\n", temp->award_level);
            printf("��ʱ��: %d-%02d-%02d\n", temp->award_time.year, temp->award_time.month, temp->award_time.day);
            printf("����:\n");
            for (int i = 0; i < 10; ++i) {
            	if(temp->awardee[i][0]=='\0')
            		break;
                printf("%d. %s\n", i + 1, temp->awardee[i]);
            }
            printf("\n"); // ��ӿ��зָ���ͬ�ľ�����Ϣ
        }
        temp = temp->next;
    }

    // ���δ�ҵ�ָ�����Ƶľ���
    if (!found) {
        printf("δ�ҵ����� %s\n", competition_name);
    }
}

// ��Ӵ���Ŀ
void addInnovate( char project_name[], char code[],int num, char member[][100], char instructor[], struct time initiation_time, struct time completion_time) {//struct innovate_node* head_ref
    // �����µĴ���Ŀ�ڵ�
    struct innovate_node* new_innovate = (struct innovate_node*)malloc(sizeof(struct innovate_node));
    if (new_innovate == NULL) {
        printf("�ڴ����ʧ�� �޷���Ӵ���Ŀ\n");
        return;
    }

    // �����´���Ŀ�ڵ�Ĳ���
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

    // ���½ڵ���ӵ�����Ŀ����
    if (innovate_head == NULL) {
        innovate_head = new_innovate;
        printf("����Ŀ %s ��ӳɹ�\n", project_name);
        return;
    }

    struct innovate_node* temp = innovate_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_innovate;

    printf("����Ŀ %s ��ӳɹ�\n", project_name);
}

//ɾ������Ŀ
void deleteInnovate(char project_name[]) {
    struct innovate_node* temp = innovate_head;
    struct innovate_node* prev = NULL;

    // �������ͷ�ڵ��Ƿ���Ҫɾ���Ľڵ�
    if (temp != NULL && strcmp(temp->project_name, project_name) == 0) {
        innovate_head = temp->next;
        free(temp);
        printf("����Ŀ %s ɾ���ɹ�\n", project_name);
        return;
    }

    // ���������ҵ�Ҫɾ���Ľڵ�
    while (temp != NULL && strcmp(temp->project_name, project_name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // ����ҵ�Ҫɾ���Ľڵ㣬��ɾ����
    if (temp != NULL) {
        prev->next = temp->next;
        free(temp);
        printf("����Ŀ %s ɾ���ɹ�\n", project_name);
    }
    else {
        printf("δ�ҵ�����Ŀ %s\n", project_name);
    }
}

// �޸Ĵ���Ŀ
void modifyInnovate(struct innovate_node* head, char project_name[], char new_project_name[], char new_code[], int num,char new_member[][100], char new_instructor[], struct time new_initiation_time, struct time new_completion_time) {
    struct innovate_node* temp = head;

    // �ҵ�Ҫ�޸ĵĴ���Ŀ�ڵ�
    while (temp != NULL && strcmp(temp->project_name, project_name) != 0) {
        temp = temp->next;
    }

    // ����ҵ�Ҫ�޸ĵĽڵ㣬������޸�
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
        printf("����Ŀ %s �޸ĳɹ�\n", project_name);
    }
    else {
        printf("δ�ҵ�����Ŀ %s\n", project_name);
    }
}

// ��ѯ����Ŀ
void queryInnovate(struct innovate_node* head, char project_name[]) {
    struct innovate_node* temp = head;
    int found = 0;

    // �����������ָ�����ƵĴ���Ŀ
    while (temp != NULL) {
        if (strcmp(temp->project_name, project_name) == 0) {
            found = 1;
            // �������Ŀ����ϸ��Ϣ
            printf("����Ŀ����: %s\n", temp->project_name);
            printf("��Ŀ���: %s\n", temp->code);
            printf("ָ����ʦ: %s\n", temp->instructor);
            printf("����ʱ��: %d-%02d-%02d\n", temp->initiation_time.year, temp->initiation_time.month, temp->initiation_time.day);
            printf("����ʱ��: %d-%02d-%02d\n", temp->completion_time.year, temp->completion_time.month, temp->completion_time.day);
            printf("��Ŀ��Ա:\n");
            for (int i = 0; i < 5; i++) {
            	if(temp->member[i][0] == '\0')
            		break;
                printf("%d. %s\n", i + 1, temp->member[i]);
            }
            printf("\n");
        }
        temp = temp->next;
    }

    // ���δ�ҵ�����Ŀ����Ϣ
    if (!found) {
        printf("δ�ҵ�����Ŀ %s\n", project_name);
    }
}

void admin_check_paper()
{
	printf("����������ѡ��:\n");
    printf("**************************************\n");
    printf("* [1] ����������Ŀ  [2] �޸�������Ϣ *\n");
    printf("* [3] ɾ��������Ŀ  [4] ��ѯ������Ŀ *\n");
    printf("* [5] �����ϼ�ѡ��  [*] ************ *\n");
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
        	printf("��������Ҫ�޸�������Ŀ������:\n");
        	gets(paper);
        	if(strlen(paper)==0){
        		printf("�������Ʋ�����Ϊ�� ����������\n");
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
	        printf("��������Ҫɾ��������Ŀ������:\n");
	        gets(paper);
	        if(strlen(paper)==0)
	        	printf("�������Ʋ�����Ϊ�� ����������\n");
	        else
				break; 
    	}
        DeleteList(paper);
    }
    else if (choice1 == 4) {
    	system("cls");
        char paper[1000];
        while(1){
	        printf("��������Ҫ��ѯ������Ŀ������:\n");
	        gets(paper);
	        if(strlen(paper)==0)
	        	printf("�������Ʋ�����Ϊ�� ����������\n");
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
        printf("������������ ����������\n");
		system("pause");
        system("cls");
        admin_check_paper();
        return;
    }
}

void admin_check_innovate()
{
	printf("����������ѡ��:\n");
    printf("**************************************\n");
    printf("* [1] ��������Ŀ  [2] �޸Ĵ󴴵ȼ� *\n");
    printf("* [3] ɾ������Ŀ  [4] ��ѯ����Ŀ *\n");
    printf("* [5] �����ϼ�ѡ��  [*] ************ *\n");
    printf("**************************************\n");
    int choice2;
    char choice2_c[100]; 
    gets(choice2_c);
    choice2=atoi(choice2_c);
    if (choice2 == 1) {
    	system("cls");
    	char project_name[1000];
		while(1){ 
	        printf("������Ҫ��ӵĴ���Ŀ����\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("����Ŀ���Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char code[100];
		while(1){
	        printf("��������Ŀ���\n");
	        gets(code);
	        if(strlen(code)==0)
	        	printf("��Ŀ��Ų�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char num_c[100];
		int num=0; 
        while (1)
        {	
			printf("��������Ŀ����\n");
            gets(num_c);
			num=atoi(num_c);
            if (num > 0 && num <= 5) {
                break;
            }
            else printf("����������� ����������\n");
        }
        printf("�밴˳�����������Ա����\n");
        char member[5][100];
        for (int i = 0; i < num; i++) {
        	while(1){
        		gets(member[i]);
        		if(strlen(member[i])==0)
					printf("������Ա���Ʋ�����Ϊ�� ����������\n");
				else
					break; 
			}
        }
		char instructor[100];
		while(1){
	        printf("������ָ����ʦ����\n");
	        gets(instructor);
	        if(strlen(instructor)==0)
	        	printf("ָ����ʦ���Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char year_c[100];
		char month_c[100];
		char day_c[100];
		struct time init_time; 
    	while(1){
	        printf("�밴������������ʼʱ��\n");
		    gets(year_c);
		    init_time.year=atoi(year_c);
		    gets(month_c);
		    init_time.month=atoi(month_c);
		    gets(day_c);
		    init_time.day=atoi(day_c);
		    if(check_time(init_time))
		    	break;
		    else
		    	printf("��ʼʱ������Ƿ� ����������\n");
		}
		struct time com_time;
		while(1){
	        printf("�밴�������������ʱ��\n");
		    gets(year_c);
		    com_time.year=atoi(year_c);
		    gets(month_c);
		    com_time.month=atoi(month_c);
		    gets(day_c);
		    com_time.day=atoi(day_c);
		    if(check_time(com_time))
		    	break;
		    else
		    	printf("����ʱ������Ƿ� ����������\n");
		}
        addInnovate(project_name, code, num,member, instructor, init_time, com_time);
    	printf("���������ϼ�\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
	}
    else if (choice2 == 2) {
    	system("cls");
		char project_name[1000];
		while(1){
	        printf("������Ҫ�޸ĵĴ���Ŀ����\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("��Ŀ���Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char new_project_name[1000];
        while(1){
			printf("�������µĴ���Ŀ����\n");
	        gets(new_project_name);
	        if(strlen(new_project_name)==0)
	        	printf("��Ŀ���Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char code[100];
		while(1){
	        printf("�������µ���Ŀ���\n");
	        gets(code);
	        if(strlen(code)==0)
	        	printf("��Ŀ��Ų�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		int num = 0;
        char num_c[100];
        printf("�������µ���Ŀ����\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 5) {
                break;
            }
            else printf("�������� ����������\n");
        }
        printf("�밴˳�����������Ա����\n");
        char member[5][100];
        for (int i = 0; i < num; i++) {
        	while(1){ 
            	gets(member[i]);
            	if(strlen(member[i])==0)
            		printf("������Ա������Ϊ�� ����������\n");
            	else
            		break;
        	}
        }
		char instructor[100];
		while(1){
	        printf("������ָ����ʦ����\n");
	        gets(instructor);
	        if(strlen(instructor)==0)
	        	printf("ָ����ʦ���Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char year_c[100];
		char month_c[100];
		char day_c[100];
		struct time init_time;
		while(1){
	        printf("�밴������������ʼʱ��\n");
		    gets(year_c);
		    init_time.year=atoi(year_c);
		    gets(month_c);
		    init_time.month=atoi(month_c);
		    gets(day_c);
		    init_time.day=atoi(day_c);
		    if(check_time(init_time))
		    	break;
		    else
		    	printf("��ʼʱ������Ƿ� ����������\n");
		}
		struct time com_time;
		while(1){
	        printf("�밴�������������ʱ��\n");    
		    gets(year_c);
		    com_time.year=atoi(year_c);
		    gets(month_c);
		    com_time.month=atoi(month_c);
		    gets(day_c);
		    com_time.day=atoi(day_c);
		    if(check_time(com_time))
		    	break;
		    else
		    	printf("����ʱ������Ƿ� ����������\n");
		}
        modifyInnovate(innovate_head, project_name, new_project_name, code,num,member, instructor, init_time, com_time);
    	printf("���������ϼ�\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
    }
    else if (choice2 == 3) {
    	system("cls");
    	char project_name[1000];
    	while(1){ 
	        printf("������Ҫɾ���Ĵ���Ŀ����\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
				printf("����Ŀ���Ʋ�����Ϊ�� ����������\n");
			else
				break; 
    	}
        deleteInnovate(project_name);
    	printf("���������ϼ�\n");
		system("pause");
		system("cls");
		admin_check_innovate();
		return;
    }
    else if (choice2 == 4) {
    	system("cls");
		char project_name[1000];
        while(1){
			printf("������Ҫ��ѯ�Ĵ���Ŀ����\n");
	        gets(project_name);
	        if(strlen(project_name)==0)
	        	printf("����Ŀ���Ʋ�����Ϊ�� ����������\n");
			else
				break; 
    	}
        queryInnovate(innovate_head, project_name);
    	printf("���������ϼ�\n");
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
        printf("����������������������\n");
        system("pause");
		system("cls");
        admin_check_innovate();
        return;
    }
}

void admin_check_competition(){
	printf("����������ѡ��:\n");
    printf("**************************************\n");
    printf("* [1] ����������Ŀ  [2] �޸ľ����ȼ� *\n");
    printf("* [3] ɾ��������Ŀ  [4] ��ѯ������Ŀ *\n");
    printf("* [5] �����ϼ�ѡ��  [*] ************ *\n");
    printf("**************************************\n");
    int choice3;
    char choice3_c[100];
    gets(choice3_c);
    choice3=atoi(choice3_c);
    if (choice3 == 1) {
    	system("cls");
    	char competition_name[1000];
    	while(1){ 
	        printf("������Ҫ��ӵľ�������\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
	        	printf("�������Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
		char organizer[100];
        while(1){
			printf("���������췽\n");
	        gets(organizer);
	        if(strlen(organizer)==0)
	        	printf("���췽������Ϊ�� ����������\n");
	        else
	        	break;
    	}
    	char award_level[100];
    	while(1){
	        printf("�����뽱��ȼ�\n");
	        gets(award_level);
	        if(strlen(award_level)==0)
	        	printf("����ȼ�������Ϊ�� ����������\n");
			else
				break; 
    	}
		int num = 0;
        char num_c[100]; 
        printf("�����뾺����������\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("�������� ����������\n");
        }
        printf("�밴˳�����������Ա����\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
        	while(1){
            	gets(awardee[i]);
            	if(strlen(awardee[i])==0)
            		printf("������Ա���Ʋ�����Ϊ�� ����������\n");
            	else
            		break;
			}
        }
		char year_c[100];
		char month_c[100];
		char day_c[100];
        struct time award_time;
		while(1){
	        printf("�밴�����������ʱ��\n");
		    gets(year_c);
		    award_time.year=atoi(year_c);
		    gets(month_c);
		    award_time.month=atoi(month_c);
		    gets(day_c);
		    award_time.day=atoi(day_c);
		    if(check_time(award_time))
		    	break;
		    else
		    	printf("��ʱ������Ƿ� ����������\n");
		}
        addCompetition(competition_name, organizer, award_level, num,awardee, award_time);
		printf("���������ϼ�\n");
		system("pause");
		system("cls");
		admin_check_competition();
		return; 
    }
    else if (choice3 == 2) {
    	system("cls");
		char competition_name[1000];
        while(1){ 
			printf("������Ҫ�޸ĵľ�������\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
	        	printf("�������Ʋ�����Ϊ�� ����������\n");
	        else
	        	break;
    	}
    	int num = 0;
        char num_c[100];
        printf("�����뾺����������\n");
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("�������� ����������\n");
        }
        printf("�밴˳�����������Ա����\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
        	while(1){
            	gets(awardee[i]);
            	if(strlen(awardee[i])==0)
            		printf("������Ա���Ʋ�����Ϊ�� ����������\n");
            	else
            		break;
        	}
        }
		char new_award_level[100];
        while(1){
	        printf("�������µĻ񽱵ȼ�\n");    
	        gets(new_award_level);
	        if(strlen(new_award_level)==0)
	        	printf("�񽱵ȼ�������Ϊ�� ����������\n");
	        else
	        	break; 
    	}
        modifyCompetition(competition_name, awardee, num,new_award_level);
		printf("���������ϼ�\n");
		system("pause");
		system("cls");
		admin_check_competition();
		return; 
    }
    else if (choice3 == 3) {
    	system("cls");
        printf("������Ҫɾ���ľ�������\n");
        char competition_name[100];
        gets(competition_name);
        printf("�����뾺����������\n");
        int num = 0;
        char num_c[100];
        while (1)
        {
            gets(num_c);
            num=atoi(num_c);
            if (num > 0 && num <= 10) {
                break;
            }
            else printf("�������� ����������\n");
        }
        printf("�밴˳�����������Ա����\n");
        char awardee[10][100];
        for (int i = 0; i < num; i++) {
            while(1){
				gets(awardee[i]);
				if(strlen(awardee[i])==0)
					printf("������Ա���Ʋ�����Ϊ�� ����������\n");
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
			printf("������Ҫ��ѯ�ľ�������\n");
	        gets(competition_name);
	        if(strlen(competition_name)==0)
				printf("�������Ʋ�����Ϊ�� ����������\n");
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
        printf("����������������������\n");
        system("pause");
        system("cls");
        admin_check_competition();
        return;
    }
}

void admin_check_GPA_add(){
	system("cls");
    printf("��ӭ�������ʼӷֹ���ϵͳ\n");
    printf("����������ѡ��:\n");
    printf("******************************\n");
    printf("* [1] ���Ĺ���  [2] �󴴹��� *\n");
    printf("* [3] ��������  [4] �����ϼ� *\n");
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
        printf("��������ȷ��ѡ��\n");
        system("pause");
        system("cls");
        admin_check_GPA_add();
        return;
    }
}

//ѧ����¼ϵͳ
void student_login(){
	char major[100];
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("������ѧ��רҵ ѧ�ż�����:\n");
        while(1){
        	gets(major);
        	if(strlen(major)==0)
        		printf("רҵ������Ϊ�� ����������\n");
			else
				break; 
    	}
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("ѧ��������� ����������\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("���벻����Ϊ�� ����������\n");
        	else
        		break;
    	}
        struct student*stu=student_login_verify(major,ID,password);
        if(stu!=NULL){
                logged_in=1;
				logged_in_stu=stu;
                system("cls");
                printf("ѧ�� %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_stu->name);
			    printf("*******************************************\n");
			    printf("* [1] �ϱ����ʼӷ�  [2] ��ѯ�γ̳ɼ���GPA *\n");
			    printf("* [3] ��ѯ��������  [4] �޸ĸ�����Ϣ      *\n");
			    printf("* [5] �����ϲ�ѡ��  [*] ************      *\n");
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
                    printf("��������ȷ��ѡ��\n");
                    system("pause");
                    system("cls");
                    student_login();
                }
            }
        else{
            printf("�����ڴ�רҵ��ѧ��������� ����������\n");
            system("pause");
            system("cls");
            student_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("ѧ�� %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_stu->name);
	    printf("*******************************************\n");
	    printf("* [1] �ϱ����ʼӷ�  [2] ��ѯ�γ̳ɼ���GPA *\n");
	    printf("* [3] ��ѯ��������  [4] �޸ĸ�����Ϣ      *\n");
	    printf("* [5] �����ϲ�ѡ��  [*] ************      *\n");
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
            printf("��������ȷ��ѡ��\n");
            system("pause");
            system("cls");
            student_login();
        }
    }
}

//��ʦ��¼ϵͳ
void teacher_login(){
	char major[100]; 
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("�������ʦרҵ ���ż�����:\n");
        while(1){
        	gets(major);
        	if(strlen(major)==0)
        		printf("רҵ������Ϊ�� ����������\n");
			else
				break; 
    	}
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("����������� ����������\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("���벻����Ϊ�� ����������\n");
        	else
        		break;
    	}
        struct teacher*tea=teacher_login_verify(major,ID,password);
        if(tea!=NULL){
                logged_in=1;
				logged_in_tea=tea;
                system("cls");
                printf("��ʦ %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_tea->name);
			    printf("***************************************\n");
			    printf("* [1] �γ̳ɼ�����  [2] �γ̳ɼ�����  *\n");
			    printf("* [3] �޸ĸ�����Ϣ  [4] �����ϲ�      *\n");
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
                    printf("��������ȷ��ѡ��\n");
                    system("pause");
                    system("cls");
                    teacher_login();
                }
            }
        else{
            printf("�����ڴ�רҵ�򹤺�������� ����������\n");
            system("pause");
            system("cls");
            teacher_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("��ʦ %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_tea->name);
	    printf("***************************************\n");
	    printf("* [1] �γ̳ɼ�����  [2] �γ̳ɼ�����  *\n");
	    printf("* [3] �޸ĸ�����Ϣ  [4] �����ϲ�      *\n");
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
            printf("��������ȷ��ѡ��\n");
            system("pause");
            system("cls");
            teacher_login();
        }
    }
}

//�����¼ϵͳ
void admin_login(){
    char ID[100];
    char password[100];
    int choice;
    char choice_c[100];
    if(logged_in==0){
    	system("cls");
        printf("��������񹤺ż�����:\n");
    	while(1){
			gets(ID);
			if(strlen(ID)!=8)
				printf("����������� ����������\n");
			else
				break;
		}
		while(1){
        	gets(password);
        	if(strlen(password)==0)
        		printf("���벻����Ϊ�� ����������\n");
        	else
        		break;
    	}
        struct admin*adm=admin_login_verify(ID,password);
        logged_in_adm=adm;
        if(adm!=NULL){
                logged_in=1;
                system("cls");
                printf("���� %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_adm->name);
			    printf("***************************************\n");
			    printf("* [1] �鿴���ʼӷ�  [2] ���ʼӷ����  *\n");
			    printf("* [3] ������������  [4] תרҵ����    *\n");
			    printf("* [5] �޸ĸ�����Ϣ  [6] �����ϲ�ѡ��  *\n");
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
                    printf("��������ȷ��ѡ��\n");
                    system("pause");
                    system("cls");
                    admin_login();
                }
            }
        else{
            printf("�˺Ż�������� ����������\n");
            system("pause");
            system("cls");
            admin_login();
        }
    }
    else{
        logged_in=1;
        system("cls");
        printf("���� %s ��½�ɹ� ��ѡ��ִ�й���:\n",logged_in_adm->name);
	    printf("***************************************\n");
	    printf("* [1] �鿴���ʼӷ�  [2] ���ʼӷ����  *\n");
	    printf("* [3] ������������  [4] תרҵ����    *\n");
	    printf("* [5] �޸ĸ�����Ϣ  [6] �����ϲ�ѡ��  *\n");
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
            printf("��������ȷ��ѡ��\n");
            system("pause");
            system("cls");
            admin_login();
        }
    }
}

//������
int main(){
    system_initialization();
    screen();
    return 0;
}