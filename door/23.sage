B=GF(23)
F.<x>=B[]
f=x^4+x^3+x+1
b=5;
bb=f(int(b))
aa=inverse_mod(int(bb),23)
a=f(int(b))%23
c=(f(int(b))*(int(inverse_mod(int(a),23))))%23
d=-int(b)*(int(f(int(b))*aa))
print("a=",b,"f(a)=",a,"inv(f)=",inverse_mod(int(a),23),"f(inv(a))=",c,"invf(a)",b+d)

print((int(b)*(int(a))%23)%23)
print(inverse_mod(int(b),23)*f(int(b))%23)
print((int(b)*inverse_mod(int(a),23)%23)%23)
print((inverse_mod(int(b),23)*inverse_mod(int(a),23)%23)%23)

b= -b
a=f(int(b))%23
print((int(b)*(int(a))%23)%23)
print(inverse_mod(int(b),23)*f(int(b))%23)
print((int(b)*inverse_mod(int(a),23)%23)%23)
print((inverse_mod(int(b),23)*inverse_mod(int(a),23)%23)%23)
