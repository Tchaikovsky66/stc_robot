
#define 增加 1
#define 减少 0

void InitValue(unsigned char address, unsigned int data);
void 从迪文更新数据(void);
void 从缓存区发送数据给迪文(void);
void 给迪文上传数据(unsigned char address, unsigned int data);
void 获取迪文所有数据(void);
void 从接收缓冲区更新数据(void);
void 恢复出厂设置(void);
void 获取所有工件名称(void);
void 将型号发送给迪文(void);
void 从缓存更新型号(void);
void 发送型号(unsigned int 迪文地址, unsigned char 型号数组地址, unsigned char 长度);
void 发送中文(unsigned int address, char *text, unsigned char len);
void 发送序号(unsigned int 工件序号);
unsigned char 保存数据(unsigned int num);
unsigned char 切换数据(unsigned int num);
void Blick(void);
void 更新图标H(__bit 增加或减少, char 图标);
void 更新图标L(__bit 增加或减少, char 图标);
