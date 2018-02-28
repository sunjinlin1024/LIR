package util

type BinaryTreeNode struct{
	minInx uint
	maxInx uint
	list map[int]interface{}
	children [2]*BinaryTreeNode
}

type BinaryTree struct{
	BinaryTreeNode
	depth uint
}	

func (self *BinaryTree)Append(id int,obj interface{},idx uint){
	self.list[id]=obj

	curParent := &self.BinaryTreeNode
	curIndex := idx
	for k:=uint(1);k<=self.depth;k++{
		if (idx&(1<<(self.depth-k)))!=0{
			curIndex=1
		}else{
			curIndex=0
		}
		curParent.children[curIndex].list[id]=obj
		curParent=curParent.children[curIndex]
		// log.Infof("id=%d,index=%d,flat=%v,val=%v,depth:%d=%d",id,idx,(1<<(self.depth-k)),(idx&(1<<(self.depth-k))),k,curIndex)
	}
}

func (self *BinaryTree)All()map[int]interface{}{
	return self.list
}

func (self *BinaryTree)FindInArea(min uint,max uint,findHandle func(interface{},bool)){
	var call func(parent *BinaryTreeNode)
	minIn:=false
	maxIn:=false
	call=func(parent *BinaryTreeNode){
		if parent.minInx>max || parent.maxInx<min{
			return
		}
		//children node is all in area  
		//leaf node is part in  area
		minIn=parent.maxInx<=max
		maxIn=parent.minInx>=min
		if (minIn && maxIn) ||
			(parent.children[0]==nil && parent.children[1]==nil){
			for _, obj := range parent.list {
				findHandle(obj,!(minIn&&maxIn))
			}
			return
		}	
		if parent.children[0]!=nil{
			call(parent.children[0])
		}
		if parent.children[1]!=nil{
			call(parent.children[1])
		}
	}
	call(&self.BinaryTreeNode)
}

func (self *BinaryTree)Remove(id int,oldIdx int)int{
	if oldIdx<0 {
		return -1
	}
	delete(self.list,id)
	curIndex:=0
	curParent:=&self.BinaryTreeNode
	for k:=uint(1);k<=self.depth;k++{
		if oldIdx&(1<<(self.depth-k))!=0{
			curIndex=1
		}else{
			curIndex=0
		}
		delete(curParent.children[curIndex].list,id)
		curParent=curParent.children[curIndex]
	}
	return int(oldIdx)
}

func InitChildren(leftDeph uint,node *BinaryTreeNode,min uint,max uint){
	// countMap[leftDeph]+=2
	node.minInx=min
	node.maxInx=max-1
	node.children[0]=&BinaryTreeNode{list:make(map[int]interface{})}
	node.children[1]=&BinaryTreeNode{list:make(map[int]interface{})}
	subArea:=(max-min)>>1
	if leftDeph>1 {
		InitChildren(leftDeph-1,node.children[0],min,max-subArea)
		InitChildren(leftDeph-1,node.children[1],min+subArea,max)
	}else{
		node.children[0].minInx=min
		node.children[0].maxInx=max-subArea-1
		node.children[1].minInx=min+subArea
		node.children[1].maxInx=max-1
	}
}

func NewBinaryTree(depth uint)*BinaryTree{
	var tree *BinaryTree = &BinaryTree{depth:depth}
	tree.list=make(map[int]interface{})
	InitChildren(depth,&tree.BinaryTreeNode,0,1<<depth)
	return tree
}