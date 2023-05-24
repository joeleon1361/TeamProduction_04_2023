#include "Collision.h"

using namespace DirectX;

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	// 中心点の距離の２乗 <= 半径の和の２乗なら交差
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// Aの半径が0の時座標はBの中心Bの半径が0の時座標はAの中心となるよう補完 
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		// 押し出すベクトルを計算
		if (reject) {
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = XMVector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	// 座標系の原点から球の中心座標への距離
	XMVECTOR distV = XMVector3Dot(sphere.center, plane.normal);
	// 平面の原点距離を減算することで、平面と球の中心との距離が出る
	float dist = distV.m128_f32[0] - plane.distance;
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > sphere.radius) return false;

	// 疑似交点を計算
	if (inter) {
		// 平面上の最近接点を、疑似交点とする
		*inter = -dist * plane.normal + sphere.center;
	}

	return true;
}

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// pointがp0の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	XMVECTOR d1 = XMVector3Dot(p0_p1, p0_pt);
	XMVECTOR d2 = XMVector3Dot(p0_p2, p0_pt);

	if (d1.m128_f32[0] <= 0.0f && d2.m128_f32[0] <= 0.0f)
	{
		// p0が最近傍
		*closest = triangle.p0;
		return;
	}

	// pointがp1の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p1_pt = point - triangle.p1;

	XMVECTOR d3 = XMVector3Dot(p0_p1, p1_pt);
	XMVECTOR d4 = XMVector3Dot(p0_p2, p1_pt);

	if (d3.m128_f32[0] >= 0.0f && d4.m128_f32[0] <= d3.m128_f32[0])
	{
		// p1が最近傍
		*closest = triangle.p1;
		return;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1.m128_f32[0] * d4.m128_f32[0] - d3.m128_f32[0] * d2.m128_f32[0];
	if (vc <= 0.0f && d1.m128_f32[0] >= 0.0f && d3.m128_f32[0] <= 0.0f)
	{
		float v = d1.m128_f32[0] / (d1.m128_f32[0] - d3.m128_f32[0]);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	XMVECTOR p2_pt = point - triangle.p2;

	XMVECTOR d5 = XMVector3Dot(p0_p1, p2_pt);
	XMVECTOR d6 = XMVector3Dot(p0_p2, p2_pt);
	if (d6.m128_f32[0] >= 0.0f && d5.m128_f32[0] <= d6.m128_f32[0])
	{
		*closest = triangle.p2;
		return;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5.m128_f32[0] * d2.m128_f32[0] - d1.m128_f32[0] * d6.m128_f32[0];
	if (vb <= 0.0f && d2.m128_f32[0] >= 0.0f && d6.m128_f32[0] <= 0.0f)
	{
		float w = d2.m128_f32[0] / (d2.m128_f32[0] - d6.m128_f32[0]);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	float va = d3.m128_f32[0] * d6.m128_f32[0] - d5.m128_f32[0] * d4.m128_f32[0];
	if (va <= 0.0f && (d4.m128_f32[0] - d3.m128_f32[0]) >= 0.0f && (d5.m128_f32[0] - d6.m128_f32[0]) >= 0.0f)
	{
		float w = (d4.m128_f32[0] - d3.m128_f32[0]) / ((d4.m128_f32[0] - d3.m128_f32[0]) + (d5.m128_f32[0] - d6.m128_f32[0]));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	XMVECTOR p;
	// 球の中心に対する最近接点である三角形上にある点ｐを見つける
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// 点pと球の中心の差分ベクトル
	XMVECTOR v = p - sphere.center;
	// 距離の二乗を求める
	// （同じベクトル同士の内積は三平方の定理のルート内部の式と一致する）
	float distanceSquare = XMVector3Dot(v, v).m128_f32[0];
	// 球と三角形の距離半径以下なら当たっていない
	if (distanceSquare > sphere.radius * sphere.radius) return false;
	// 擬似交点を計算
	if (inter) {
		// 三角形上の最近接点ｐを擬似交点とする
		*inter = p;
	}
	// 押し出すベクトルを計算
	if (reject) {
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面去線とレイの方向ベクトルの内積
	float d1 = XMVector3Dot(plane.normal, ray.dir).m128_f32[0];
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離（平面の法線方向）
	// 面法線とレイの始点座標（位置ベクトル）の内積
	float d2 = XMVector3Dot(plane.normal, ray.start).m128_f32[0];
	// 始点と平面の距離（平面の法線方向）
	float dist = d2 - plane.distance;
	// 始点と平面の距離（レイ方向）
	float t = dist / -d1;
	// 交点が始点より後ろにあるので、当たらない
	if (t < 0) return false;
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	if (inter) { *inter = ray.start + t * ray.dir; }
	return true;
}

bool Collision::CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	// 三角形が乗っている平面を算出
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];

	// レイと平面が当たっていなければ、当たっていない
	if (!CheckRay2Plane(ray, plane, distance, &interPlane)) { return false; }

	// レイと平面が当たっていたので、距離と交点が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	XMVECTOR m;

	// 辺p0_p1について
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);

	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// 辺p1_p2について
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// 辺p2_p0について
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	// 辺の外側であれば当たっていないので判定を打ち切る
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// 内側なので、当たっている
	if (inter) {
		*inter = interPlane;
	}

	return true;
}

bool Collision::CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = ray.start - sphere.center;
	float b = XMVector3Dot(m, ray.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// layの始点がsphereの外側にあり(c > 0)、layがsphereから離れていく方向を差している場合(b > 0)、当たらない 
	if (c > 0.0f && b > 0.0f) return false;

	float discr = b * b - c;
	// 負の判別式はレイが球を外れていることに一致
	if (discr < 0.0f) { return false; }

	// レイは球と交差している
	// 交差する最小の値tを計算
	float t = -b - sqrtf(discr);
	// tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if (t < 0) t = 0.0f;
	if (distance) { *distance = t; }

	if (inter) { *inter = ray.start + t * ray.dir; }

	return true;
}

bool Collision::CheckRay2OBB(const Ray& ray, const Box& obb, float* distance, DirectX::XMVECTOR* inter)
{
	Plane plane;
	float scale[3] = { obb.scale.x ,obb.scale.y ,obb.scale.z };
	bool result = false;
	float resultDistance = D3D12_FLOAT32_MAX;
	XMVECTOR resultInter = {};
	int resultInt = 0;
	//OBB6面分,面とレイの判定を行う
	for (int i = 0; i < 6; i++)
	{
		XMVECTOR otherDir[2];
		//iが0,3の時はX方向1,4のときはY方向2,5のときはZ方向
		int val2 = i % 3;
		if (val2 == 0)
		{
			otherDir[0] = obb.normalDirect[1] * obb.scale.y;
			otherDir[1] = obb.normalDirect[2] * obb.scale.z;
		}
		else if (val2 == 1)
		{
			otherDir[0] = obb.normalDirect[0] * obb.scale.x;
			otherDir[1] = obb.normalDirect[2] * obb.scale.z;
		}
		else
		{
			otherDir[0] = obb.normalDirect[0] * obb.scale.x;
			otherDir[1] = obb.normalDirect[1] * obb.scale.y;
		}

		//0-2は軸に対して正の方向、3-5は負の方向
		int val = i / 3;
		plane.normal = obb.normalDirect[i - 3 * val] * (1 - 2 * val);

		XMVECTOR p0 = otherDir[0] + otherDir[1] + obb.normalDirect[i - 3 * val] * (1 - 2 * val) * scale[i - 3 * val] + obb.center;

		plane.distance = XMVector3Dot(plane.normal, p0).m128_f32[0];
		float dis;
		XMVECTOR interPlane;
		// レイと平面が当たっていなければ、当たっていない	
		if (!CheckRay2Plane(ray, plane, &dis, &interPlane)) {
			continue;
		}
		//衝突距離が既存のものより小さかったら情報を書き込む
		if (resultDistance > dis)
		{
			resultDistance = dis;
			resultInter = interPlane;
			resultInt = i;
		}
		result = true;
	}
	if (!result)
		return false;

	//0-2は軸に対して正の方向、3-5は負の方向
	int val = resultInt / 3;
	//iが0,3の時はX方向1,4のときはY方向2,5のときはZ方向
	int val2 = resultInt % 3;
	auto normal = obb.normalDirect[resultInt - 3 * val] * (1 - 2 * val);
	const float epsilon = 1.0e-5f;	// 誤差吸収用の微小な値
	float normalScale = {};
	XMVECTOR otherDir[2];

	if (val2 == 0)
	{
		normalScale = obb.scale.x;
		otherDir[0] = obb.normalDirect[1] * obb.scale.y;
		otherDir[1] = obb.normalDirect[2] * obb.scale.z;
	}
	else if (val2 == 1)
	{
		normalScale = obb.scale.y;
		otherDir[0] = obb.normalDirect[0] * obb.scale.x;
		otherDir[1] = obb.normalDirect[2] * obb.scale.z;
	}
	else
	{
		normalScale = obb.scale.z;
		otherDir[0] = obb.normalDirect[0] * obb.scale.x;
		otherDir[1] = obb.normalDirect[1] * obb.scale.y;
	}
	XMVECTOR p[4];
	p[0] = otherDir[0] + otherDir[1] + normal * normalScale + obb.center;
	p[1] = otherDir[0] - otherDir[1] + normal * normalScale + obb.center;
	p[2] = -otherDir[0] - otherDir[1] + normal * normalScale + obb.center;
	p[3] = -otherDir[0] + otherDir[1] + normal * normalScale + obb.center;


	//右回り左回りどちらかでレイが内側を通っていたらOK
	bool check1 = true;
	bool check2 = true;

	XMVECTOR m;
	// 辺p0_p1について
	XMVECTOR pt_p0 = p[0] - resultInter;
	XMVECTOR p0_p1 = p[1] - p[0];
	m = XMVector3Cross(pt_p0, p0_p1);
	// 辺の外側
	if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
		check1 = false;
	}

	if (check1)
	{
		// 辺p1_p2について
		XMVECTOR pt_p1 = p[1] - resultInter;
		XMVECTOR p1_p2 = p[2] - p[1];
		m = XMVector3Cross(pt_p1, p1_p2);
		// 辺の外側
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (check1)
	{
		// 辺p2_p3について
		XMVECTOR pt_p2 = p[2] - resultInter;
		XMVECTOR p2_p3 = p[3] - p[2];
		m = XMVector3Cross(pt_p2, p2_p3);
		// 辺の外側
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (check1)
	{
		// 辺p3_p0について
		XMVECTOR pt_p3 = p[3] - resultInter;
		XMVECTOR p3_p0 = p[0] - p[3];
		m = XMVector3Cross(pt_p3, p3_p0);
		// 辺の外側
		if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
			check1 = false;
		}
	}
	if (!check1)
	{
		if (check2)
		{
			// 辺p0_p1について
			XMVECTOR pt_p0 = p[0] - resultInter;
			XMVECTOR p0_p1 = p[0] - p[1];
			m = XMVector3Cross(pt_p0, p0_p1);
			// 辺の外側
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// 辺p1_p2について
			XMVECTOR pt_p1 = p[1] - resultInter;
			XMVECTOR p1_p2 = p[1] - p[2];
			m = XMVector3Cross(pt_p1, p1_p2);
			// 辺の外側
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// 辺p2_p3について
			XMVECTOR pt_p2 = p[2] - resultInter;
			XMVECTOR p2_p3 = p[2] - p[3];
			m = XMVector3Cross(pt_p2, p2_p3);
			// 辺の外側
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
		if (check2)
		{
			// 辺p3_p0について
			XMVECTOR pt_p3 = p[3] - resultInter;
			XMVECTOR p3_p0 = p[3] - p[0];
			m = XMVector3Cross(pt_p3, p3_p0);
			// 辺の外側
			if (XMVector3Dot(m, normal).m128_f32[0] < -epsilon) {
				check2 = false;
			}
		}
	}
	//右回り左回りっどちらも外側を通っていたら当たっていない
	if (!check1 && !check2)
		return false;

	if (inter)
		*inter = resultInter;

	if (distance)
		*distance = resultDistance;

	// 内側なので、当たっている
	return true;
}

// OBB v.s. OBB
bool Collision::CheckOBBOBB(const Box& box1, const Box& box2, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{

	auto scale1 = box1.scale;
	auto scale2 = box2.scale;

	// 各方向ベクトルの確保
	// （N***:標準化方向ベクトル）
	Vector3 NAe1 = box1.normalDirect[0];
	Vector3 NAe2 = box1.normalDirect[1];
	Vector3 NAe3 = box1.normalDirect[2];
	Vector3 NBe1 = box2.normalDirect[0];
	Vector3 NBe2 = box2.normalDirect[1];
	Vector3 NBe3 = box2.normalDirect[2];
	Vector3 Ae1 = NAe1 * scale1.x;
	Vector3 Ae2 = NAe2 * scale1.y;
	Vector3 Ae3 = NAe3 * scale1.z;
	Vector3 Be1 = NBe1 * scale2.x;
	Vector3 Be2 = NBe2 * scale2.y;
	Vector3 Be3 = NBe3 * scale2.z;
	Vector3 Interval = box1.center - box2.center;

	float rejectLength = 0;
	Vector3 rejectVec = {};

	// 分離軸 : Ae1
	float rA = Ae1.Length();
	float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabs(Vector3::Dot(Interval, NAe1));
	if (L > rA + rB)
		return false; // 衝突していない

	// 分離軸 : Ae2
	rA = Ae2.Length();
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe2));
	if (L > rA + rB)
		return false;

	// 分離軸 : Ae3
	rA = Ae3.Length();
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, NAe3));
	if (L > rA + rB)
		return false;

	// 分離軸 : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.Length();
	L = fabs(Vector3::Dot(Interval, NBe1));
	if (L > rA + rB)
		return false;
	rejectLength = rB + rA - L;
	rejectVec = NBe1;

	// 分離軸 : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.Length();
	L = fabs(Vector3::Dot(Interval, NBe2));
	if (L > rA + rB)
		return false;
	if (rejectLength > rB + rA - L)
	{
		rejectLength = rB + rA - L;
		rejectVec = NBe2;
	}

	// 分離軸 : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.Length();
	L = fabs(Vector3::Dot(Interval, NBe3));
	if (L > rA + rB)
		return false;
	if (rejectLength > rB + rA - L)
	{
		rejectLength = rB + rA - L;
		rejectVec = NBe3;
	}

	// 分離軸 : C11
	Vector3 Cross;
	Cross = Vector3::Cross(NAe1, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C12
	Cross = Vector3::Cross(NAe1, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C13
	Cross = Vector3::Cross(NAe1, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C21
	Cross = Vector3::Cross(NAe2, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C22
	Cross = Vector3::Cross(NAe2, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C23
	Cross = Vector3::Cross(NAe2, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C31
	Cross = Vector3::Cross(NAe3, NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C32
	Cross = Vector3::Cross(NAe3, NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	// 分離軸 : C33
	Cross = Vector3::Cross(NAe3, NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabs(Vector3::Dot(Interval, Cross));
	if (L > rA + rB)
		return false;

	if (reject)
	{
		//排斥ベクトルとBox1-2の方向ベクトルが逆向きなら排斥ベクトルを逆向きにする
		float dot = rejectVec.Dot(Vector3::Normalize(Interval));
		if (dot < 0)
			rejectVec *= -1;

		*reject = Vector3(rejectVec * rejectLength).ConvertXMVECTOR();
	}
	// 分離平面が存在しないので「衝突している」
	return true;
}

float Collision::LenSegOnSeparateAxis(const Vector3& Sep, const Vector3& e1, const Vector3& e2, const Vector3& e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	float r1 = fabs(Vector3::Dot(Sep, e1));
	float r2 = fabs(Vector3::Dot(Sep, e2));
	float r3;
	if (e3.x != 0 || e3.y != 0 || e3.z != 0)
	{
		r3 = fabs(Vector3::Dot(Sep, e3));
	}
	else
		r3 = 0;
	return r1 + r2 + r3;
}
