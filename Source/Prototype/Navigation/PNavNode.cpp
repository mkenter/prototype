// Fill out your copyright notice in the Description page of Project Settings.


#include "PNavNode.h"

int UPNavNode::GetFCost() const
{
	return GCost + HCost;
}
