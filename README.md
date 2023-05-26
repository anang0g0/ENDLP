# Key exchange by semidirect Decomposition Problem 
## 警告
この方式で行列を使うと線形代数攻撃で解読できます。
なぜなら、A1,A2で同じ元を使っている２つの式を合わせれば、未知変数の個数と一致して、全ての秘密鍵が計算できるからです。それに対抗するためには、まだどのような組み合わせが安全かわかりませんが、線形代数以外の群を使った半直積を試してみてください。
例えば、x,yを楕円曲線の点を成分に持つ行列として、それ以外の$a,b,c,d$を整数行列にするなど、線形代数が使えないようにした変な行列で試してください。
問題は行列以外の群に何を選ぶかです。もしそれが行列でないなら、それを使って安全な鍵交換ができるはずです。

## 行列以外の群に対する鍵交換
（とは言え行列以外の非可換軍って少ないのよね・・・。）

ここではブレイド群を含めた一般の非可換群に対する鍵交換プロトコルを紹介します。
この問題は、私が離散対数でもなく共役元探索問題でもない、非可換群を用いた鍵交換で初めて経験した方法です。
世の中非可換群を使ったプロトコルなんて山のようにあるけど、本当は可換になる部分群を使ってたりしてがっかりすることが多いのですが、そんな偽物ではなく本当に非可換な元同士を組み合わせて鍵交換をしようというのが動機です。
行列を使った暗号は色々攻撃法があって危険かもしれませんが、そのときは半直積にしてお好きな群を非可換にしてみるのも手だと思います。
ちなみに置換群は行列なので解読できる可能性が高いです。
今度記事を書くときには、置換群と数ベクトルの半直積を作って、その弱点を調べたいと思います。
今まで置換群で共役元探索問題をやろうとして線形代数攻撃に悩まされてきたのですが、今度こそその攻撃を交わすことができればいいと思います。
ちなみに行列の３分割問題への攻撃法は先程思いつきましたので、整数行列は絶対に使わないでください。
多変数多項式を要素に持つベクトルと、整数ベクトルの半直積などをやってみたいです。
例えば、１変数多項式をｎ本持つベクトルと、ｎこの整数を持つベクトルとの半直積を考える。
ベクトルを$v,x$、多項式を$f,g$とすると$(v,f)(x,g)=(f_n(x_n)+v_n,f_ng_n)$（ここで$Z/mZ$は整数環,$I(x)$は多項式の法となる既約多項式）のような演算を定義します。

# Decomposition Problem

Problem 2.(Decomposition Problem)とは、 
ある$v = x^{-1}a_2x_2,a$が与えられた時、$ , x1 ∈ H, a ∈ A, x2 ∈ H'.$
（要するに$x1,x2$が冪等元でないこと）
を満たすような$x_1,x_2$を求める問題です。

ここでは3分割のうち1つが公開されている問題になっているはずです。
見た感じ行列を使った暗号によく使われるようです。
ちょっと線形代数攻撃を試してみましょう。
今$a$が公開されていて$x_1,x_2$が秘密であり、
$v=x_1ax_2$
であったとしましょう。そのとき、未知の元$x_2$の逆元を右からかけると、
$vx_2^{-1}=x_1a$
この条件をみたすような$x_1,x_2^{-1}$を計算する問題がこの暗号の強度になります。

次にまず、アリスは次のように非可換群の元を取ります。
例えばｎ時の対称群$S_n$にします。

アリスは6個の異なる元を取ります。
$a,b,c,d,x,y \in S_n$
ボブも同様に６個の元を取ります。
アリスはそのうち$x,y$だけを公開し、アリスとボブとの間で共有します。
公開する鍵はボブが決めても問題ありません。
つまりボブの秘密鍵は、
$e,f,g,h' \in S_n$
の４つだけです。

# 鍵設定
アリスは次のようにして公開鍵を作ります。
まず$d^{-1}a=h'^{-1}e=\phi$であるように決めます。
これは後で述べるビットパターンの分離記号としての役割を果たす元です。

$A1=(a_1=axb,a_2=b^{-1}xc,a_3=c^{-1}xd^{-1})$
$A2=(b_1=ayb,b_2=b^{-1}yc,b_3=c^{-1}yd^{-1})$

$B1=(c_1=exf,c_2=f^{-1}xg,c_3=g^{-1}xh'^{-1})$
$B2=(d_1=eyf,d_2=f^{-1}yg,d_3=g^{-1}y h'^{-1})$

$A1,A2$はアリスの公開鍵、$B1,B2$がボブの公開鍵、$a,b,c,d$がアリスの秘密鍵、$e,f,g,h'$がボブの秘密鍵です。

# ビットパターンと鍵共有
（ややこしいのでじっくり読みましょうｗ）

ここで1つ制約条件をつけます。
それはアリスとボブの秘密のバイナリ列の長さが偶数であるという仮定です。
その仮定のもとにこの鍵交換は成り立ちます。
ビットパターンを
$h=(0,0),i=(0,1),j=(1,0),k=(1,1)$とします。
長さ2のバイナリ列にはこの4パターンしかないので、これだけ定義できれば十分です。

この時、各2ビット列が、アリストボブの間でどのように組み合わせられるのか見ていきます。
まず、パターンhのとき、アリスの鍵の組み合わせは、
$h=a_1a_2a_3=ax^3d^{-1}$と決めます。
同様にパターンiの場合は、
$i=a_1b_2a_3=axyxd^{-1}$
$j=b_1a_2b_3=ayxyd^{-1}$
$k=b_1b_2b_3=ay^3d^{-1}$

となるように決めます。
同様にボブも、
$h1=c_1c_2c_3=ex^3h'^{-1}$と決めます。
同様にパターンiの場合は、
$i1=c_1d_2c_3=exyxh'^{-1}$
$j1=d_1c_2d_3=eyxyh'^{-1}$
$k1=d_1d_2d_3=ey^3h'^{-1}$

これは行ってしまえば、０と１を2つの元の対応関係で表現しているということです。
3つの異なる元を組み合わせると同時に、ビット列を共有したいという要求を実現できます。

ここで$d^{-1}a=h^{-1}e=\phi$と決めた事を思い出してください。
あとはパターン通りに鍵の組み合わせを並べるだけです。

アリスのビットパターンが1010のときは、
$K_{a_1}=jj=b_1a_2b_3b_1a_2b_3=ayxy\phi yxyd^{-1}$
$K_{a_2}=j1j1=c_1d_2c_1b_1d_2b_3=eyxy\phi yxyh'^{-1}$
1111の場合は、
$K_{b_1}=kk=b_1b_2b_3b_1b_2b_3=ay^3\phi y^3d^{-1}$
$K_{b_2}=k1k1=ey^3\phi y^3h'^{-1}$

$K_{alice}=K_{a_1}K_{b_1}=ayxy\phi yxy^4\phi y^3d^{-1}$
$K_{bob}=K_{a_2}K_{b_2}=eyxy\phi yxy^4\phi y^3h'^{-1}$

であり、鍵に割り当てられた番号同士をつなぎ合わせて、$K_{alice},K_{bob}$からそれぞれアリスとボブの秘密鍵を取り除けば鍵共有は出来上がりです。
これは非可換でなければならないという要請に答えるための鍵共有ができる元の3分割問題(triple decomposition problem)を利用したものとなると思います。（不安）

ここで問題が。
$\phi$が公開されることで、特定の位置に$\phi$があることがわかるというのはどの程度問題になるだろうか？
かと言って1にしてしまうと3つのパーツを全部揃えれば線形代数攻撃に弱くなるし、何かいい方法があるかもう少し考えます。

成功したら大発見です。

