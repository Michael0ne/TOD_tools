class Model_Animator : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Model_Animator", "Node", false, false);
		
		//	TODO: add members and methods to Model_Animator.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Model_Animator::Create();