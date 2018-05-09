#include <iostream>
#include "parse.hpp"

namespace orrery {

void print_unit(const unit_t &u){
	if(u.begin == u.end) std::cout<<"(empty)";
	for(auto it = u.begin; it<u.end; it++){
		std::cout<<it->s<<" ";
	}
}

void parse(unit_t &unit){
	for(auto it = unit.begin; it!=unit.end; it++){
		if(it->s == "import" || it->s == "return"){
			std::cout<<it->s;
			it++;
			unit_t u;
			u.begin = it;
			while(it->t != type::ExprEnd) it++;
			u.end = it;
			std::cout<<"(";
			print_unit(u);
			std::cout<<")"<<std::endl;
		}else if(it->s == "fn" || it->s == "if"){
			std::cout<<it->s<<":"<<std::endl;
			it++;
			unit_t head, blk;
			head.begin = head.end = it;
			while(head.end->t != type::BlkStart) head.end++;
			blk.begin = blk.end = head.end+1;
			while(blk.end->t != type::BlkEnd) blk.end++;

			std::cout<<"\theader: ";
			print_unit(head);
			std::cout<<std::endl<<"\tblock: ";
			print_unit(blk);
			std::cout<<std::endl;

			parse(blk);

			it = blk.end;
		}else{
			unit_t u;
			u.begin = u.end = it;
			while(u.end->t != type::ExprEnd) u.end++;
			std::cout<<"expr: ";
			print_unit(u);
			std::cout<<std::endl;

			it = u.end;
		}
	}
}

}
