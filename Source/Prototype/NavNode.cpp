// Fill out your copyright notice in the Description page of Project Settings.


#include "NavNode.h"

int UNavNode::GetFCost() const
{
	return GCost + HCost;
}
