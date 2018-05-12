#include <iostream>
#include <stack>
#include "parse.hpp"

namespace orrery {

void print_unit(const unit_t &u){
	if(u.begin == u.end) std::cout<<"(empty)";
	for(auto it = u.begin; it<u.end; it++){
		std::cout<<it->s<<" ";
	}
}

std::string unit2str(const unit_t &u){
	if(u.begin == u.end) return "(empty)";
	std::string s;
	for(auto it = u.begin; it<u.end; it++){
		s += it->s;
		s += " ";
	}
	return s;
}

size_t get_priority(const token_t &op){
	const auto s = op.s;
	size_t p = 0;
	if(s == "*" || s == "/") p=2;
	else if(s == "+" || s == "-") p=1;
	return p;
}

void parse_expr(unit_t &unit){
	std::vector<unit_t> v;
	std::vector<token_t> op;
	std::vector<token_t> buf;
	std::stack<token_t> st;

	// 逆ポーランド記法に変換する
	for(auto it=unit.begin; it<unit.end; it++){
		if(it->t == type::SubStart) st.push(*it);
		else if(it->t == type::SubEnd){
			while(true){
				auto t = st.top();
				st.pop();
				if(t.t == type::SubStart) break;
				buf.push_back(t);
			}
		}else if(it->t == type::Operator){
			if(st.empty()) st.push(*it);
			else if(get_priority(*it) > get_priority(st.top())){
				st.push(*it);
			}else{
				buf.push_back(st.top());
				st.pop();
				st.push(*it);
			}
		}else buf.push_back(*it);
	}
	while(!st.empty()){
		buf.push_back(st.top());
		st.pop();
	}

	std::cout<<"逆ポ: ";
	for(auto &t : buf){
		std::cout<<t.s<<" ";
	}
	std::cout<<std::endl;

	std::stack<int> s;
	for(auto &t : buf){
		if(t.t == type::Operator){
			int v2 = s.top(); s.pop();
			int v1 = s.top(); s.pop();
			int ret = 0;
			if(t.s == "+") ret=v1+v2;
			else if(t.s == "*") ret=v1*v2;
			else std::cout<<"not impl"<<std::endl;
			s.push(ret);
			std::cout<<ret<<" <- "<<v1<<" "<<t.s<<" "<<v2<<std::endl;
		}else{
			int v = std::stoi(std::string(t.s));
			s.push(v);
		}
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
			parse_expr(u);

			it = u.end;
		}
	}
}

}
