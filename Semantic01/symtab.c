/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object* obj);
void freeScope(Scope* scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab* symtab;
Type* intType;
Type* charType;

/******************* Type utilities ******************************/

Type* makeIntType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type* makeCharType(void) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type* makeArrayType(int arraySize, Type* elementType) {
  Type* type = (Type*) malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type* duplicateType(Type* type) {
  // TODO
  Type *tmp = (Type*)malloc(sizeof(Type));
  tmp->typeClass = type->typeClass;
  tmp->elementType = type->elementType;
  tmp->arraySize = type->arraySize;

  return tmp;
}

int compareType(Type* type1, Type* type2) {
  // TODO
  if(type1->typeClass == type2->typeClass)
  {
    if(type1->typeClass == TP_ARRAY)
    {
      if(type1->arraySize != type2->arraySize ||
      compareType(type1->elementType,type2->elementType) == 0)
      {
        return 0;
      }
      return 1;
    }
  }
  return 0;
}

void freeType(Type* type) {
  // TODO
  if(type->elementType != NULL)
  {
    freeType(type->elementType);
  }
  free(type);
}

/******************* Constant utility ******************************/

ConstantValue* makeIntConstant(int i) {
  // TODO
  ConstantValue *IntConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  IntConstantValue->type = TP_INT;
  IntConstantValue->intValue = i;
  return IntConstantValue;
}

ConstantValue* makeCharConstant(char ch) {
  // TODO
  ConstantValue *CharConstantValue = (ConstantValue*)malloc(sizeof(ConstantValue));
  CharConstantValue->type = TP_CHAR;
  CharConstantValue->intValue = ch;
  return CharConstantValue;
}

ConstantValue* duplicateConstantValue(ConstantValue* v) {
  // TODO
  ConstantValue *tmp = (ConstantValue*)malloc(sizeof(ConstantValue));
  tmp->type = v->type;
  if(v->type == TP_CHAR)
  {
    tmp->intValue = v->charValue;
  }
  else if(v->type == TP_INT)
  {
    tmp->charValue = v->intValue;
  }
  return tmp;
}

/******************* Object utilities ******************************/

Scope* CreateScope(Object* owner, Scope* outer) {
  Scope* scope = (Scope*) malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object* CreateProgramObject(char *programName) {
  Object* program = (Object*) malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes*) malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program,NULL);
  symtab->program = program;

  return program;
}

Object* CreateConstantObject(char *name) {
  // TODO
  Object *constant = (Object*)malloc(sizeof(Object));
  strcpy(constant->name,name);
  constant->kind = OBJ_CONSTANT;
  constant->constAttrs = (ConstantAttributes*)malloc(sizeof(ConstantAttributes));
  constant->constAttrs->value = (ConstantValue*)malloc(sizeof(ConstantValue));
  return constant;
}

Object* CreateTypeObject(char *name) {
  // TODO
  Object * type = (Object*)malloc(sizeof(Object));
  strcpy(type->name,name);
  type->kind = OBJ_TYPE;
  type->typeAttrs = (TypeAttributes*)malloc(sizeof(TypeAttributes));
  type->typeAttrs->actualType = (Type*)malloc(sizeof(Type));
  
  return type;
}

Object* CreateVariableObject(char *name) {
  // TODO
  Object *var = (Object*)malloc(sizeof(Object));
  strcpy(var->name,name);
  var->kind = OBJ_VARIABLE;
  var->varAttrs = (VariableAttributes*)malloc(sizeof(VariableAttributes));
  var->varAttrs->type = (Type*)malloc(sizeof(Type));
  var->varAttrs->scope = CreateScope(var,symtab->currentScope);
  return var;
}

Object* CreateFunctionObject(char *name) {
  // TODO
  Object* func = (Object*)malloc(sizeof(Object));
  strcpy(func->name,name);
  func->kind = OBJ_FUNCTION;
  func->funcAttrs = (FunctionAttributes*)malloc(sizeof(FunctionAttributes));
  func->funcAttrs->returnType = (Type*)malloc(sizeof(Type));
  func->funcAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));
  func->funcAttrs->scope = CreateScope(func,symtab->currentScope);
  return func;
}

Object* CreateProcedureObject(char *name) {
  // TODO
  Object *proce = (Object*)malloc(sizeof(Object));
  strcpy(proce->name,name);
  proce->kind = OBJ_PROCEDURE;
  proce->procAttrs = (ProcedureAttributes*)malloc(sizeof(ProcedureAttributes));
  proce->procAttrs->paramList = (ObjectNode*)malloc(sizeof(ObjectNode));
  proce->procAttrs->scope = CreateScope(proce,symtab->currentScope);
  return proce;
}

Object* CreateParameterObject(char *name, enum ParamKind kind, Object* owner) {
  // TODO
  Object*param = (Object*)malloc(sizeof(Object));
  strcpy(param->name,name);
  param->kind = OBJ_PARAMETER;
  param->paramAttrs = (ParameterAttributes*)malloc(sizeof(ParameterAttributes));
  param->paramAttrs->function = owner;
  param->paramAttrs->kind = kind;
  param->paramAttrs->type = (Type*)malloc(sizeof(Type));
  return param;
}

void freeObject(Object* obj) {
  // TODO
  
  if(obj!=NULL)
  {
      switch (obj->kind)
      {
      case OBJ_CONSTANT:
        if(obj->constAttrs->value != NULL)
        {
          free(obj->constAttrs->value);
        }
        free(obj->constAttrs);
        break;
      case OBJ_VARIABLE:
        if(obj->varAttrs->type !=NULL)
        {
          free(obj->varAttrs->type);
        }
        if(obj->varAttrs->scope != NULL)
        {
          free(obj->varAttrs->scope);
        }
        free(obj->varAttrs);
        break;
      case OBJ_TYPE:
        if(obj->typeAttrs->actualType != NULL)
        {
          free(obj->typeAttrs->actualType);
        }
        free(obj->typeAttrs);
        break;
      case OBJ_PROGRAM:
        if(obj->progAttrs->scope !=NULL)
        {
          free(obj->progAttrs->scope);
        }
        free(obj->procAttrs);
        break;
      case OBJ_FUNCTION:
        if(obj->funcAttrs->paramList != NULL) 
        {
          freeReferenceList(obj->funcAttrs->paramList);
        }
        if(obj->funcAttrs->returnType != NULL) 
        {
          freeType(obj->funcAttrs->returnType);
        }
        if(obj->funcAttrs->scope != NULL) 
        { 
          freeScope(obj->funcAttrs->scope);
        }
        free(obj->funcAttrs);
        break;
      case OBJ_PROCEDURE:
        if(obj->procAttrs->paramList != NULL) 
        {
          freeReferenceList(obj->procAttrs->paramList);
        } 
        if(obj->procAttrs->scope != NULL) 
        {
          freeScope(obj->procAttrs->scope);
        }
        free(obj->procAttrs);
        break;
      case OBJ_PARAMETER:
        if(obj->paramAttrs->type !=NULL)
        {
          free(obj->paramAttrs->type);
        }
        else if(obj->paramAttrs->function != NULL)
        {
          free(obj->paramAttrs->function);
        }
        free(obj->paramAttrs);
        break;
      default:
        break;
      }
    free(obj);
  }
}

void freeScope(Scope* scope) {
  // TODO
  if(scope != NULL)
  {
    freeObjectList(scope->objList);
    free(scope);
    scope = NULL;
  }
}

void freeObjectList(ObjectNode *objList) {
  // TODO
  if(objList !=NULL)
  {
    freeObject(objList->object);
    freeObjectList(objList->next);
    objList = NULL;
  }
}

void freeReferenceList(ObjectNode *objList) {
  // TODO
  ObjectNode *temp = objList;
  while(objList != NULL)
  {
    temp = objList;
    objList = objList->next;
    free(temp);
  }
}

void AddObject(ObjectNode **objList, Object* obj) {
  ObjectNode* node = (ObjectNode*) malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL) 
    *objList = node;
  else {
    ObjectNode *n = *objList;
    while (n->next != NULL) 
      n = n->next;
    n->next = node;
  }
}

Object* findObject(ObjectNode *objList, char *name) {
  // TODO
  ObjectNode *currentObject = objList;
  while(currentObject != NULL)
  {
    if(strcmp(currentObject->object->name,name) == 0)
    {
      return currentObject->object;
    }
    currentObject = currentObject->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void) {
  Object* obj;
  Object* param;

  symtab = (SymTab*) malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;
  
  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList),param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void) {
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope* scope) {
  symtab->currentScope = scope;
}

void exitBlock(void) {
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object* obj) {
  if (obj->kind == OBJ_PARAMETER) {
    Object* owner = symtab->currentScope->owner;
    switch (owner->kind) {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }
 
  AddObject(&(symtab->currentScope->objList), obj);
}


