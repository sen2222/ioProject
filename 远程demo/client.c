#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    int doctrIndex; // 对应医生下标
    char name[15];
    char sex[3]; // 性别
} __attribute__((packed)) patient;

void showMeun(void)
{
    printf("============================\n");
    printf("====1、挂号请点击回车键=====\n");
    printf("====2、退出程序============= \n");
    printf("============================\n");
}

void showDoctr(void)
{
    printf("============================\n");
    printf("=====1、戴夫医生(儿科)======\n");
    printf("=====2、玛丽亚医生(妇产科)==\n");
    printf("=====3、维尔逊医生(神经科)==\n");
    printf("=====4、扁鹊医生(骨科)======\n");
    printf("=====5、怀特医生(内科)======\n");
    printf("=====6、约翰医生(外科)======\n");
    printf("============================\n");
}

// 清除输入缓冲区中的所有字符，直到遇到换行符
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

int main(void)
{
    char tmp;
    patient *pit = (patient *)malloc(sizeof(patient));

    while (1)
    {
        FILE *fd = fopen("./io/pit", "ab+"); // 2追加内容
        showMeun();
        clearInputBuffer(); // 清除缓冲区
        scanf("%c", &tmp);

        if (tmp == '2')
        {
            break; // 用户选择退出
        }

        showDoctr();
        // 选医生科目
        printf("请选择您要挂的科(输入序号1-6): ");

        scanf("%d", &pit->doctrIndex);
        clearInputBuffer();                              // 清除缓冲区
        printf("选中的医生编号: %d\n", pit->doctrIndex); // 检查选中的医生编号是否正确

        // 输入名字
        printf("请输入您的姓名：");
        scanf("%s", pit->name);

        clearInputBuffer();

        // 输入性别
        printf("请输入您的性别: ");
        scanf("%s", pit->sex);

        // 写入文件前清空缓冲区
        clearInputBuffer();
        pit->doctrIndex--;

        size_t written = fwrite(pit, sizeof(patient), 1, fd);
        if (written != 1)
        {
            perror("Failed to write to file");
        }
        else
        {
            fflush(fd);
            printf("挂号成功！\n");
        }

        fclose(fd);
        sleep(5);
    }

    free(pit);

    printf("程序已退出。\n");
    return 0;
}
