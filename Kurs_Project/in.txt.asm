declare integer function one(integer a,unsigned integer b)|
{|
output '������� one ���������';|
call date();|
call time();|
declare integer d=a;|
declare unsigned integer e=b;|
declare integer res;|
res=(d*10);|
return res;|
};|
main|
{|
declare integer g=10;|
declare unsigned integer k=20;|
declare integer h;|
h=one(g,k);|
output h;|
if(h<10;)|
{|
output '������ 10!';|
};|
output '������� �� ��������!';|
};|
