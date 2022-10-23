#include <QCoreApplication>
#include <QDebug>
#include <QCryptographicHash>
#include <iostream>
#include <QList>
#include <QFile>
#include <QThread>
#include <QTime>
    //97->122



class mythread : public QThread
 {
 public:
     mythread();
     void run();
     void setData(int _sPos, int _fPos,QStringList _listHash);
     QString getPass;
     int sPos;
     int fPos;
     QStringList listHash;

 };

mythread::mythread()
{

}
void mythread::setData(int _sPos, int _fPos, QStringList _listHash)
{
    sPos=_sPos;
    fPos=_fPos;
    listHash=_listHash;
}

void mythread::run()
{
   // qDebug()<<QString("thread  start: %1 end: %2").arg(sPos).arg(fPos);
    QString test;
    QString hash;
    for(int i=97+sPos;i<=97+fPos;i++){
        for(int j=97;j<=122;j++){
            for(int k=97;k<=122;k++){
                for(int m=97;m<=122;m++){
                    for(int l=97;l<=122;l++){
                        test=QString("%1%2%3%4%5").arg(char(i)).arg(char(j)).arg(char(k)).arg(char(m)).arg(char(l));
                        QString hash=QCryptographicHash::hash(test.toUtf8(), QCryptographicHash::Sha256).toHex();
                        if(listHash.contains(hash)){
                            qDebug()<<test;
                            // qDebug()<<QString("Нашел thread  start: %1 end: %2").arg(sPos).arg(fPos);
                        }
                    }
                }
            }
        }
    }
}







QStringList pass(int sPos, int fPos,int len, QStringList hashList){
    QFile file("allPass.txt");
    file.open(QIODevice::WriteOnly);
    QString test;
    QString ex;
    int count =0;
    bool step=true;
    ex.fill('z',len);
    QStringList passwords;
    test.fill('a',len);
    while(true){
        count++;
        /*QString hash=QCryptographicHash::hash(test.toUtf8(), QCryptographicHash::Sha256).toHex();
        if(hashList.contains(hash)){
            passwords.append(test);
            qDebug()<<passwords.length();
        }*/
        test+='\n';
         file.write(test.toUtf8());
        if(test==ex)
            break;
        for(int j=1;j<=len;j++){
            if((test[j]=='z')&&(test[j-1]!='z')){
                if(j==len-1)
                    step=false;
                test[j-1]=char(test[j-1].toLatin1()+1);
                test[j]='a';
            }
        }
        if(step)
            test[len-1]=char(test[len-1].toLatin1()+1);
        else
            step=true;
         qDebug()<<test;

        }
    qDebug()<<count;
     return passwords;
    }


void allPas(){
    QFile file("allPass.txt");
    file.open(QIODevice::WriteOnly);
    int count=0;
    for(int i=97;i<=122;i++){
        for(int j=97;j<=122;j++){
            for(int k=97;k<=122;k++){
                for(int l=97;l<=122;l++){
                            file.write((QString("%1%2%3%4\n").arg(char(i)).arg(char(j)).arg(char(k)).arg(char(l))).toUtf8());
                        }


            }
        }
    }
    file.close();
    qDebug()<<count;
}
void  counter(){
    int count=0;
    count++;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"Russian");
    int count;
    while(true){
        system("cls");
        std::cout<<"number of threads:\n";
        std::cin>>count;
        if((count>0)&&(count<8))
            break;
        else
            std::cout<<"1<thread<7";
    }
    int step,lastStep=0,last;
    step=26/count-1;
    last=26%count;
    QCoreApplication a(argc, argv);
    QStringList listHash;
    QFile file("sha.txt");
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        listHash.append(file.readLine().trimmed());
    }
    file.close();
    mythread *thread=new mythread[count];
    for(int i=0;i<count;i++){
        if(last>0){
            thread[i].setData(lastStep,lastStep+step+1,listHash);
            lastStep+=step+2;
            last--;
        }
        else
        {
            thread[i].setData(lastStep,lastStep+step,listHash);
            lastStep+=step+1;
        }
    }
    QTime t;
    t.start();
    for(int i=0;i<count;i++){
       thread[i].start();
    }
    int countFin=0;;
    while(1){
        for(int i=0;i<count;i++){
           if(thread[i].isFinished())
               countFin++;
        }
        if(countFin==count)
            break;
        else
            countFin=0;
    }
    qDebug("Прошло: %d ms", t.elapsed());
}
