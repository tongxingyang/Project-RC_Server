#ifndef _GAMESTRUCT_SCENE_H_
#define _GAMESTRUCT_SCENE_H_

#include "Base.h"
#include "GameDefine_Scene.h"
#include "DBStruct/DBStruct.h"

//////////////////////////////////////////////////////////////////////////
typedef int SceneClassID;		//��������ID
typedef int SceneInstID;			//����ʵ��ID

//����ID�����ĳ������ĳ��ʵ��
struct SceneID
{
	SceneID(void)
	{
		m_nClassID = invalid_id;
		m_nInstID = invalid_id;
	}
	SceneID(SceneClassID nClassID, SceneInstID nInstID)
	{
		m_nClassID = nClassID;
		m_nInstID = nInstID;
	}
	SceneClassID	m_nClassID;
	SceneInstID		m_nInstID;
};

//////////////////////////////////////////////////////////////////////////
class MarchSceneInfo
{
public:
	MarchSceneInfo(void) {CleanUp();}
	~MarchSceneInfo(void) {}
public:
	MarchSceneInfo(const MarchSceneInfo &r)
	{
		m_nState = r.m_nState;
		m_SceneID = r.m_SceneID;
	}
	MarchSceneInfo& operator = (const MarchSceneInfo &r)
	{
		if (this != &r)
		{
			m_nState = r.m_nState;
			m_SceneID = r.m_SceneID;
		}
		return *this;
	}
public:
	void CleanUp(void)
	{
		m_nState = invalid_id;
		m_SceneID = SceneID(invalid_id, invalid_id);
	}
public:
	enum
	{
		SCENEPLAYING = 0,
		SCENECHANGING,
	};
public:
	int m_nState;
	SceneID m_SceneID;
};

class MarchBaseInfo
{
public:
	MarchBaseInfo(void) {CleanUp();}
	~MarchBaseInfo(void) {}
public:
	MarchBaseInfo(const MarchBaseInfo &r)
	{
		m_Guid = r.m_Guid;
		m_szName = r.m_szName;
		m_nLevel = r.m_nLevel;
		m_nCombatNum = r.m_nCombatNum;
	}
	MarchBaseInfo& operator = (const MarchBaseInfo &r)
	{
		if (this != &r)
		{
			m_Guid = r.m_Guid;
			m_szName = r.m_szName;
			m_nLevel = r.m_nLevel;
			m_nCombatNum = r.m_nCombatNum;
		}
		return *this;
	}
public:
	void CleanUp(void)
	{
		m_Guid = 0;
		m_szName = "";
		m_nLevel = 0;
		m_nCombatNum = 0;
	}
public:
	int64		m_Guid;
	CHARNAME	m_szName;
	int		m_nLevel;
	int		m_nCombatNum;
};

class MarchInfo
{
public:
	MarchInfo(void) {CleanUp();}
	~MarchInfo(void) {}
public:
	MarchInfo(const MarchInfo &r)
	{
		m_MarchSceneInfo = r.m_MarchSceneInfo;
		m_MarchBaseInfo = r.m_MarchBaseInfo;
	}
	MarchInfo& operator = (const MarchInfo &r)
	{
		if (this != &r)
		{
			m_MarchSceneInfo = r.m_MarchSceneInfo;
			m_MarchBaseInfo = r.m_MarchBaseInfo;
		}
		return *this;
	}
public:
	void CleanUp(void)
	{
		m_MarchSceneInfo.CleanUp();
		m_MarchBaseInfo.CleanUp();
	}
public:
	MarchSceneInfo m_MarchSceneInfo;
	MarchBaseInfo m_MarchBaseInfo;
};
typedef std::map<int64, MarchInfo> MarchInfoMap;


//////////////////////////////////////////////////////////////////////////
class ScenePos
{
public:
	float	m_fX;
	float	m_fZ;
public:
	ScenePos(void) : m_fX(0.0f), m_fZ(0.0f) {}
	ScenePos(float fX, float fZ) : m_fX(fX), m_fZ(fZ) {}
	ScenePos(const ScenePos &r) : m_fX(r.m_fX), m_fZ(r.m_fZ) {}
	ScenePos& operator=(const ScenePos &r)
	{
		m_fX = r.m_fX;
		m_fZ = r.m_fZ;
		return *this;
	}
public:
	void CleanUp(void)
	{
		m_fX = 0.0f;
		m_fZ = 0.0f;
	}
	bool operator==(const ScenePos &r) const
	{
		return (fabs(m_fX-r.m_fX)+fabs(m_fZ-r.m_fZ)) < 0.001f;
	}
	bool operator!=(const ScenePos &r) const
	{
		return !(*this == r);
	}
	float DistanceSquare(const ScenePos &r) const
	{
		float fDistX, fDistZ;
		fDistX	= r.m_fX - m_fX;
		fDistZ	= r.m_fZ - m_fZ;
		return (float)(fDistX * fDistX + fDistZ * fDistZ);
	}
	float Distance(const ScenePos &r) const
	{
		return (float)sqrt(DistanceSquare(r));
	}
};

//////////////////////////////////////////////////////////////////////////
float CalcDirection(const ScenePos &rPosStart, const ScenePos &rPosEnd);
void NormaliseDirection(float &rDirection);

//////////////////////////////////////////////////////////////////////////
template<typename _ElemType, int _ElemSize>
class GeneralObjRefCont
{
public:
	typedef GeneralObjRefCont<_ElemType, _ElemSize> _MyselfClass;
	typedef _ElemType* _ElemPtr;
	typedef _ElemType& _ElemRef;

public:
	GeneralObjRefCont<_ElemType, _ElemSize>(void)
	{
		CleanUp();
	}
	~GeneralObjRefCont<_ElemType, _ElemSize>(void)
	{

	}

public:
	void CleanUp(void)
	{
		m_nValidSize = 0;
		memset(m_ElemPtr, 0, sizeof(m_ElemPtr));
	}

public:
	int Size(void)		const {return m_nValidSize;}					//��Ч����
	int MaxSize(void)	const {return _ElemSize;}						//�������
	bool Empty(void)		const {return (m_nValidSize <= 0);}				//�Ƿ�Ϊ��
	bool Full(void)			const {return (m_nValidSize >= _ElemSize);}		//�Ƿ�����

public:
	//�淨�߼���ʹ�ô˽ӿڣ�������ЧֵΪ[0,Size)
	_ElemRef operator[](int i)
	{
		AssertEx(m_nValidSize >= 0 && m_nValidSize <= _ElemSize, "");
		AssertEx(i >= 0 && i < m_nValidSize, "");
		AssertEx(m_ElemPtr[i] != null_ptr, "");
		return *(m_ElemPtr[i]);
	}

public:
	//���º����淨�߼���Ҫʹ�ã�����������
	//ȷ��ʹ��ʱ�����ɴ���ά����ȷ��
	bool Append(_ElemPtr ref)
	{
		if (ref != null_ptr)
		{
			if (!Full())
			{
				AssertEx(m_nValidSize >= 0 && m_nValidSize < _ElemSize, "");
				m_ElemPtr[m_nValidSize++] = ref;
				return true;
			}
		}
		return false;
	}
	template<typename _Pred>
	void RemoveByPred(const _Pred &pr)
	{
		AssertEx(m_nValidSize >= 0 && m_nValidSize <= _ElemSize, "");
		for (int i = 0; i < Size(); )
		{
			AssertEx(m_ElemPtr[i] != null_ptr, "");
			if (pr(*(m_ElemPtr[i])))
			{
				//�Ƴ���ǰԪ�غ��±꼴ָ����һ��Ԫ��
				RemoveByIndex(i);
			}
			else
			{
				//���Ƴ���ǰԪ�أ��±�������һ��Ԫ��
				i++;
			}
		}
	}
	void RemoveByIndex(int i)
	{
		AssertEx(m_nValidSize >= 0 && m_nValidSize <= _ElemSize, "");
		AssertEx(i >= 0 && i < m_nValidSize, "");
		for (int n1 = i, n2 = i + 1; n2 < m_nValidSize; n1++, n2++)
		{
			m_ElemPtr[n1] = m_ElemPtr[n2];
		}
		m_nValidSize--;
	}
	void ReduceSize(int nNewSize)
	{
		AssertEx(m_nValidSize >= 0 && m_nValidSize <= _ElemSize, "");
		AssertEx(nNewSize >= 0 && nNewSize <= m_nValidSize, "");
		m_nValidSize = nNewSize;
	}

public:
	//���º����淨�߼���Ҫʹ�ã�����������
	//ȷ��ʹ��ʱ�����ɴ���ά����ȷ��
	//begin���ص�һ����ЧԪ�أ�end�������һ����ЧԪ�صĺ�һ��Ԫ�أ���STL���ݣ����end�ķ���ֵ�Ǿ��Բ��ܽ����õ�
	_ElemPtr *begin(void)
	{
		AssertEx(m_nValidSize >= 1 && m_nValidSize <= _ElemSize, "");
		AssertEx(m_ElemPtr[0] != null_ptr, "");
		return &(m_ElemPtr[0]);
	}
	_ElemPtr *end(void)
	{
		AssertEx(m_nValidSize >= 1 && m_nValidSize <= _ElemSize, "");
		return &(m_ElemPtr[m_nValidSize]);
	}

private:
	int m_nValidSize;
	_ElemPtr m_ElemPtr[_ElemSize];
};

//////////////////////////////////////////////////////////////////////////
class ScriptValueListTraits
{
public:
	static void CleanUpElems(int *lpElems, int nSize)
	{
		if (lpElems != null_ptr)
		{
			for (int i = 0; i < nSize; i++)
			{
				lpElems[i] = invalid_id;
			}
		}
	}
	static void CleanUpElem(int &rElem)
	{
		rElem = invalid_id;
	}
	static bool ShouldSeal(void)
	{
		return false;
	}
};
typedef FLSeque<int, ScriptValueListTraits, 8> ScriptParamList;
struct ScriptReturnValue
{
	enum
	{
		RET_OK = 0,
		RET_FAILED,
		RET_FUNCTIONNOTEXIST,
	};
	int m_nRet;
	ScriptParamList m_ValueList;
};

//////////////////////////////////////////////////////////////////////////
class PathNode
{
public:
	int		m_nSerial;
	bool		m_bBroadcast;
	ScenePos	m_StartPos;
	ScenePos	m_EndPos;
public:
	PathNode(void)
		: m_StartPos()
		, m_EndPos()
	{
		m_nSerial = invalid_id;
		m_bBroadcast = false;
	}
	PathNode(const ScenePos &rps, const ScenePos &rpe)
		: m_StartPos(rps)
		, m_EndPos(rpe)
	{
		m_nSerial = invalid_id;
		m_bBroadcast = false;
	}
	PathNode(const PathNode &r)
	{
		m_nSerial = r.m_nSerial;
		m_bBroadcast = r.m_bBroadcast;
		m_StartPos = r.m_StartPos;
		m_EndPos = r.m_EndPos;
	}
	PathNode & operator = (const PathNode &r)
	{
		if (this != &r)
		{
			m_nSerial = r.m_nSerial;
			m_bBroadcast = r.m_bBroadcast;
			m_StartPos = r.m_StartPos;
			m_EndPos = r.m_EndPos;
		}
		return *this;
	}
};

class PathCont
{
public:
	enum
	{
		AVAILABLE_SIZE	= 32,
		FULL_SIZE		= AVAILABLE_SIZE + 1,
		MAX_SERIAL		= 100,
	};

private:
	int		m_nFront;
	int		m_nRear;
	bsarray<PathNode, FULL_SIZE> m_PathNodeArray;
private:
	int		m_nSerialGenerator;

public:
	PathCont(void)
	{
		m_nFront = 0;
		m_nRear = 0;
		m_nSerialGenerator = 0;
	}
private:
	PathCont(const PathCont &r);
	PathCont & operator = (const PathCont &r);

public:
	void CleanUp(void)
	{
		m_nFront = 0;
		m_nRear = 0;
	}
public:
	bool Empty(void) const
	{
		return (Size() <= 0);
	}
	bool Full(void) const
	{
		return (Size() >= MaxSize());
	}
	int MaxSize(void) const
	{
		return (FULL_SIZE - 1);
	}
	int Size(void) const
	{
		return ((FULL_SIZE + (m_nRear - m_nFront)) % FULL_SIZE);
	}
public:
	int BroadcastedSize(void) const
	{
		int nRet = 0;
		int nSize = Size();
		int nMaxSize = MaxSize();
		AssertEx(nSize >= 0 && nSize <= nMaxSize, "");
		for (int i = 0; i < nSize; i++)
		{
			if ((*this)[i].m_bBroadcast)
			{
				nRet++;
			}
		}
		return nRet;
	}
	int UnBroadcastedSize(void) const
	{
		int nRet = 0;
		int nSize = Size();
		int nMaxSize = MaxSize();
		AssertEx(nSize >= 0 && nSize <= nMaxSize, "");
		for (int i = 0; i < nSize; i++)
		{
			if (!((*this)[i].m_bBroadcast))
			{
				nRet++;
			}
		}
		return nRet;
	}
public:
	bool PushBack(const PathNode &r)
	{
		if (Full())
		{
			return false;
		}
		else
		{
			PathNode pn = r;
			pn.m_nSerial = m_nSerialGenerator;
			pn.m_bBroadcast = false;

			m_nSerialGenerator = (m_nSerialGenerator + 1) % MAX_SERIAL;

			m_PathNodeArray[m_nRear] = pn;
			m_nRear = (m_nRear + 1) % FULL_SIZE;

			return true;
		}
	}
	bool PopFront(PathNode &r)
	{
		if (Empty())
		{
			return false;
		}
		else
		{
			r = m_PathNodeArray[m_nFront];
			m_nFront = (m_nFront + 1) % FULL_SIZE;

			return true;
		}
	}
	bool PopFront(void)
	{
		PathNode pn;
		return PopFront(pn);
	}
	PathNode & operator[](int i)
	{
		int nSize = Size();
		int nMaxSize = MaxSize();
		AssertEx(nSize >= 0 && nSize <= nMaxSize, "");
		AssertEx(i >= 0 && i < nSize, "");
		int ii = (m_nFront + i) % FULL_SIZE;
		AssertEx(ii >= 0 && ii <= nMaxSize, "");
		return m_PathNodeArray[ii];
	}
	const PathNode & operator[](int i) const
	{
		int nSize = Size();
		int nMaxSize = MaxSize();
		AssertEx(nSize >= 0 && nSize <= nMaxSize, "");
		AssertEx(i >= 0 && i < nSize, "");
		int ii = (m_nFront + i) % FULL_SIZE;
		AssertEx(ii >= 0 && ii <= nMaxSize, "");
		return m_PathNodeArray[ii];
	}
	PathCont & operator+=(const PathCont &rCont)
	{
		if (this != &rCont)
		{
			int nSize = rCont.Size();
			for (int i = 0; i < nSize; i++)
			{
				PushBack(rCont[i]);
			}
		}
		return *this;
	}
};
struct CopySceneRewardItem
{

	CopySceneRewardItem(void)
	{
		Clear();
	}
	void Clear()
	{
		m_nType = invalid_id;
		m_nItemID = invalid_id;
		m_nCount = 0;
		m_nNoticeID = invalid_id;
	}
	int m_nType;
	int m_nItemID;
	int m_nCount;
	int m_nNoticeID;
};
//��������
BSARRAY_ASSIGN_DECL(CopySceneRewardItem, MAX_COPYSCENE_REWARD);
struct CopySceneReward
{
	CopySceneReward(void)
	{
		
		 Clear();
	}
	void Clear()
	{
		m_nDrawIndex = invalid_id;
		m_nDrawIndexTwo = invalid_id;
		m_nExp = 0;
		m_nMoney = 0;
		for (int i = 0; i < MAX_COPYSCENE_REWARD; ++i)
		{
			m_Reward[i].Clear();
		}
		for (int i = 0; i < MAX_COPYSCENE_DRAW; ++i)
		{
			m_Draw[i].Clear();
		}
	}
	bool AddReward(	int nType,	int nItemID, int nCount, int nNoticeID = invalid_id)
	{
		for (int i = 0; i < MAX_COPYSCENE_REWARD;++i)
		{
			if (m_Reward[i].m_nType == invalid_id)
			{
				m_Reward[i].m_nType = nType;
				m_Reward[i].m_nItemID = nItemID;
				m_Reward[i].m_nCount = nCount;
				m_Reward[i].m_nNoticeID = nNoticeID;
				return true;
			}
		}
		return false;
	}
	bool AddDraw(	int nType,	int nItemID, int nCount)
	{
		for (int i = 0; i < MAX_COPYSCENE_DRAW;++i)
		{
			if (m_Draw[i].m_nType == invalid_id)
			{
				m_Draw[i].m_nType = nType;
				m_Draw[i].m_nItemID = nItemID;
				m_Draw[i].m_nCount = nCount;
				return true;
			}
		}
		return false;
	}
	bsarray<CopySceneRewardItem, MAX_COPYSCENE_REWARD> m_Reward;
	bsarray<CopySceneRewardItem, MAX_COPYSCENE_DRAW> m_Draw;
	int m_nDrawIndex;
	int m_nDrawIndexTwo;
	int m_nExp;
	int m_nMoney;
};

//////////////////////////////////////////////////////////////////////////
struct OnlineAwardTableLine
{
public:
	int m_ID;
	int m_LeftTime;
	int m_Exp;
	int m_Money;
	int m_BindYuanbao;
	int m_Item1DataID;
	int m_Item1Count;
	int m_Item2DataID;
	int m_Item2Count;

public:
	OnlineAwardTableLine()
	{
		CleanUp();
	}
	void CleanUp()
	{
		m_ID = -1;
		m_LeftTime = -1;
		m_Exp = -1;
		m_Money = -1;
		m_BindYuanbao = -1;
		m_Item1DataID = -1;
		m_Item1Count = -1;
		m_Item2DataID = -1;
		m_Item2Count = -1;
	}
};
struct OnlineAwardDateTime
{
public:
	int m_nStartDate;
	int m_nEndDate;
	int m_nStartTime;
	int m_nEndTime;
	int m_nShowStartTime;
	int m_nShowEndTime;
public:
	OnlineAwardDateTime()
	{
		 CleanUp();
	}
	void CleanUp()
	{
		m_nStartDate = -1;
		m_nEndDate = -1;
		m_nStartTime = -1;
		m_nEndTime = -1;
		m_nShowStartTime = -1;
		m_nShowEndTime = -1;
	}
};


//�����ڲ����͵�ṹ
struct TeleportInScene 
{
public:
	int m_nId;			//���͵�ID
	float m_fPosX;		//���͵�λ��X
	float m_fPosZ;		//���͵�λ��Z
	float m_fTargetX;	//Ŀ���λ��X
	float m_fTargetZ;	//Ŀ���λ��Z

public:
	TeleportInScene()
	{
		CleanUp();
	}

	void CleanUp()
	{
		m_nId = -1;
		m_fPosX = m_fPosZ = m_fTargetX = m_fTargetZ = 0.0f;
	}

	bool IsValid()
	{
		return (m_nId >= 0);
	}
};


#endif