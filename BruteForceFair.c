#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <windows.h>//for windows version
#include <omp.h>
//此文件的说明：虽然定义上这个函数的复杂度为O(M*n),但是实际上因为内层循环的break的出现，导致了可能第一个字符就不匹配，实际复杂度为O(M-Tmiss+Tmiss*N)，超过4个核心之后加速比一直在2左右不成线形 同时也因为break的出现，导致在并行方法中内层循环无法并行化
typedef struct index_node
{
    int index;
    struct index_node * next;
} index_node;

index_node* BruteForce(const char* str, const char* match){
    int lenstr = strlen(str);
    int lenmat = strlen(match);
    index_node *head = (index_node *)malloc(sizeof(index_node));
    head->next = NULL;
    head->index = -1;
    index_node *p = head;
    for (int i = 0; i <= lenstr-lenmat; i++){
        int status = 1; //乐观锁
        for(int j = 0; j < lenmat; j++){
            if(str[i+j] != match[j]){
                status = 0;
            }
        }
        if(status == 1){
            index_node *node = (index_node *)malloc(sizeof(index_node));
            node->index = i;
            p->next = node;
            p = p->next;
            p->next=NULL;
        }
    }
    return head;
}
index_node* BruteForceOpenMP(const char* str, const char* match, int core_count){
    int lenstr = strlen(str);
    int lenmat = strlen(match);
    int index = -1;
    omp_set_num_threads(core_count);
    index_node *head = (index_node *)malloc(sizeof(index_node));
    head->next = NULL;
    head->index = -1;
    index_node *p = head;

#pragma omp parallel for
        for (int i = 0; i <= lenstr - lenmat; i++) {
            int status = 1; //本次定位的乐观锁
#pragma omp parallel for
            for (int j = 0; j < lenmat; j++) {
                if (str[i + j] != match[j]) {
                    status = 0;
                }
            }
            if (status == 1) {
                index_node *node = (index_node *) malloc(sizeof(index_node));
                node->index = i;
                p->next = node;
                p = p->next;
                p->next = NULL;
            }
        }
    return head;
}
int main(){
    //prepare the file into memory
    FILE*fp;
    fp=fopen("bbe.txt","r");
    fseek(fp,0L,SEEK_END);
    int size=ftell(fp);
    char* str =(char *)malloc(size); /* 根据文件大小动态分配内存空间 */
    if(str==NULL){
        fclose(fp);
        return 0;
        }
    fseek(fp,0L,SEEK_SET);
    fread(str,size,1,fp);
    fclose(fp);
    int cores = 8;
    //for windows version counter
    LARGE_INTEGER nBeginTime1;
    LARGE_INTEGER nBeginTime2;
    LARGE_INTEGER nBeginTime3;
    LARGE_INTEGER nEndTime1;
    LARGE_INTEGER nEndTime2;
    LARGE_INTEGER nEndTime3;
    LARGE_INTEGER nFreq;
    QueryPerformanceFrequency(&nFreq); //CPU 频率
    printf("%s","\n\n------------------------Long words matching compare------------------------\n\n");
    const char* match = "\n"
                        "Psm 70:1 &lt;To the chief music-maker. Of David. To keep in memory.&gt; Let your salvation come quickly, O God; come quickly to my help, O Lord.\n"
                        "Psm 70:2 Let those who go after my soul have shame and trouble; let those who have evil designs against me be turned back and made foolish.\n"
                        "Psm 70:3 Let those who say Aha, aha! be turned back as a reward of their shame.\n"
                        "Psm 70:4 Let all those who are looking for you be glad and have joy in you; let the lovers of your salvation ever say, May God be great.\n"
                        "Psm 70:5 But I am poor and in need; come to me quickly, O God; you are my help and my saviour; let there be no waiting, O Lord.\n"
                        "Psm 71:1 In you, O Lord, have I put my hope; let me never be shamed.\n"
                        "Psm 71:2 Keep me safe in your righteousness, and come to my help; give ear to my voice, and be my saviour.\n"
                        "Psm 71:3 Be my strong Rock, the strong place of my salvation; for you are my Rock, and my safe place.\n"
                        "Psm 71:4 O my God, take me out of the hand of the sinner, out of the hand of the evil and cruel man.\n"
                        "Psm 71:5 For you are my hope, O Lord God; I have had faith in you from the time when I was young.\n"
                        "Psm 71:6 You have been my support from the day of my birth; you took me out of my mother's body; my praise will be ever of you.\n"
                        "Psm 71:7 I am a wonder to all; but you are my strong tower.\n"
                        "Psm 71:8 My mouth will be full of your praise and glory all the day.\n"
                        "Psm 71:9 Do not give me up when I am old; be my help even when my strength is gone.\n"
                        "Psm 71:10 For my haters are waiting secretly for me; and those who are watching for my soul are banded together in their evil designs,\n"
                        "Psm 71:11 Saying, God has given him up; go after him and take him, for he has no helper.\n"
                        "Psm 71:12 O God, be not far from me; O my God, come quickly to my help.\n"
                        "Psm 71:13 Let those who say evil against my soul be overcome and put to shame; let my haters be made low and have no honour.\n"
                        "Psm 71:14 But I will go on ever hoping, and increasing in all your praise.\n"
                        "Psm 71:15 My mouth will make clear your righteousness and your salvation all the day; for they are more than may be measured.\n"
                        "Psm 71:16 I will give news of the great acts of the Lord God; my words will be of your righteousness, and of yours only.\n"
                        "Psm 71:17 O God, you have been my teacher from the time when I was young; and I have been talking of your works of wonder even till now.\n"
                        "Psm 71:18 Now when I am old and grey-headed, O God, give me not up; till I have made clear your strength to this generation, and your power to all those to come.\n"
                        "Psm 71:19 Your righteousness, O God, is very high; you have done great things; O God, who is like you?\n"
                        "Psm 71:20 You, who have sent great and bitter troubles on me, will give me life again, lifting me up from the deep waters of the underworld.\n"
                        "Psm 71:21 You will make me greater than before, and give me comfort on every side.\n"
                        "Psm 71:22 I will give praise to you with instruments of music, O my God, for you are true; I will make songs to you with music, O Holy One of Israel.\n"
                        "Psm 71:23 Joy will be on my lips when I make melody to you; and in my soul, to which you have given salvation.";
    //No OpenMP BruteForce
    QueryPerformanceCounter(&nBeginTime1);
    index_node *index1 = BruteForce(str, match);
    QueryPerformanceCounter(&nEndTime1);
    printf("the indexes of match string are in the following (from Bruteforce without OpenMP)\n");
    index1 = index1->next;
    while(index1!=NULL){
        printf("index: %d\n",index1->index);
        index1 = index1->next;
    }
    //for windows version
    double time1 = (double)(nEndTime1.QuadPart-nBeginTime1.QuadPart)/(double)nFreq.QuadPart * 1000;
    printf("time cost without OpenMP was %fms",time1);
    printf("%s","\n-------------------------------------------------\n");

    //Multiple cores compare with single core

    //With OpenMP BruteForce
    QueryPerformanceCounter(&nBeginTime2);
    struct index_node *index2 = BruteForceOpenMP(str, match,cores);
    QueryPerformanceCounter(&nEndTime2);
    printf("the indexes of match string are in the following (from Bruteforce without OpenMP)\n");
    index2 = index2->next;
    while(index2!=NULL){
        printf("index: %d\n",index2->index);
        index2 = index2->next;
    }
    double time2 = (double)(nEndTime2.QuadPart-nBeginTime2.QuadPart)/(double)nFreq.QuadPart * 1000;
    printf("time cost with OpenMP with %d cores was %fms\n\n", cores, time2);
    printf("Speed up of long word matching of OpenMP is %f",time1/time2);
    printf("%s","\n----------------------end------------------------\n");


}