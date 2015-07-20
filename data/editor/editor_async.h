/* Copyright (C) 2005-2015, Unigine Corp. All rights reserved.
 *
 * File:    editor_async.h
 * Desc:    Unigine editor
 * Version: 1.00
 * Author:  Andrey Viktorov <unclebob@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#ifndef __EDITOR_ASYNC_H__
#define __EDITOR_ASYNC_H__

/*
 */
namespace AsyncPool {
	
	/*
	 */
	class ThreadPair {
		
		Async async;
		int thread_id;
		
		ThreadPair(Async a,int id) {
			async = a;
			thread_id = id;
		}
		
		~ThreadPair() {
			async = NULL;
		}
		
		int isThread() {
			return is_thread(thread_id);
		}
		
		int isAlive() {
			if(is_thread(thread_id)) return true;
			if(async.isRunning()) return true;
			return false;
		}
		
		void kill() {
			kill_thread(thread_id);
		}
		
		void stop() {
			async.setPriority(-3);
		}
	};
	
	/*
	 */
	class NamedPool {
		
		string name;
		
		Async free_asyncs[0];
		
		int is_stopped = true;
		
		ThreadPair threads[0];
		
		NamedPool(string n) {
			name = n;
		}
		
		~NamedPool() {
			clear();
			free_asyncs.delete();
		}
		
		Async get() {
			if(free_asyncs.size() == 0) return new Async();
			
			Async async = free_asyncs[free_asyncs.size() - 1];
			async.setPriority(0);
			free_asyncs.remove();
			return async;
		}
		
		void releaseAsync(Async async) {
			free_asyncs.append(async);
		}
		
		int runThread(int function,int args[]) {
			Async async = get();
			if(async == NULL) {
				log.error(__FUNC__ + ": async is NULL\n");
				return -1;
			}
			
			args.append(0,async);
			
			if(is_function(function,args.size()) == 0) {
				log.error(__FUNC__ + ": can't find function with %d arguments\n",args.size());
				return -1;
			}
			
			int id = thread(function,args);
			threads.append(new ThreadPair(async,id));
			is_stopped = false;
			return id;
		}
		
		int getNumThreads() {
			return threads.size();
		}
		
		int getThreadId(int num) {
			return threads[num].thread_id;
		}
		
		Async getThreadAsync(int num) {
			return threads[num].async;
		}
		
		void clear() {
			forloop(int i = 0; threads.size()) {
				if(threads[i].isThread()) threads[i].kill();
				free_asyncs.append(threads[i].async);
			}
			threads.delete();
		}
		
		void stopThreads() {
			clear();
			is_stopped = true;
		}
		
		int isRunning() {
			if(is_stopped) return false;
			forloop(int i = 0; threads.size()) {
				if(threads[i].isThread()) return true;
			}
			return false;
		}
		
		
		void update() {
			for(int i = 0; i < threads.size(); i++) {
				if(threads[i].isAlive()) continue;
				Async async = threads[i].async;
				free_asyncs.append(async);
				threads.remove(i);
				i--;
			}
		}
	};
	
	NamedPool named_pools[];
	NamedPool main_pool;
	int need_stop_flags[];
	
	/*
	 */
	void shutdown_pool(NamedPool pool) {
		forloop(int i = 0; pool.getNumThreads()) {
			Async a = pool.getThreadAsync(i);
			a.wait();
		}
	}
	
	/*
	 */
	void wait_pool(NamedPool pool) {
		while(pool.isRunning()) wait;
		pool.clear();
	}
	
	/*
	 */
	void init() {
		main_pool = new NamedPool("");
		
		forloop(int i = 0; max(engine.console.getInt("cpu_count"),1)) {
			main_pool.releaseAsync(new Async());
		}
	}
	
	/*
	 */
	void shutdown() {
		shutdown_pool(main_pool);
		foreach(NamedPool pool; named_pools) {
			shutdown_pool(pool);
		}
		
		delete main_pool;
		named_pools.delete();
		need_stop_flags.clear();
	}
	
	void update() {
		main_pool.update();
		named_pools.call("update");
	}
	
	/*
	 */
	Async get() {
		return main_pool.get();
	}
	
	void releaseAsync(Async async) {
		main_pool.releaseAsync(async);
	}
	
	/*
	 */
	int runThread(int function,int args[]) {
		int id = main_pool.runThread(function,args);
		need_stop_flags.append(id,false);
		return id;
	}
	
	/*
	 */
	int runThread(int function,int args[],string name) {
		if(name == "") return main_pool.runThread(function,args);
		
		NamedPool pool = named_pools.check(name,NULL);
		if(pool == NULL) {
			pool = new NamedPool(name);
			named_pools.append(name,pool);
		}
		int id = pool.runThread(function,args);
		need_stop_flags.append(id,false);
		return id;
	}
	
	/*
	 */
	void waitThreads() {
		wait_pool(main_pool);
		foreach(NamedPool pool; named_pools) {
			wait_pool(pool);
		}
	}
	
	/*
	 */
	void waitThreads(string name) {
		if(name == "") {
			waitThreads();
			return;
		}
		
		NamedPool pool = named_pools.check(name,NULL);
		if(pool == NULL) return;
		wait_pool(pool);
	}
	
	/*
	 */
	void clearThreads() {
		main_pool.clear();
		foreach(NamedPool pool; named_pools) {
			pool.clear();
		}
	}
	
	int isStoped(string name) {
		NamedPool pool = named_pools.check(name,NULL);
		if(pool == NULL) return true;
		return pool.is_stopped;
	}
	
	int isRunning(string name) {
		NamedPool pool = named_pools.check(name,NULL);
		if(pool == NULL) return false;
		return pool.isRunning();
	}
	
	void stop(string name) {
		NamedPool pool = named_pools.check(name,NULL);
		if(pool == NULL) return;
		return pool.stopThreads();
	}
	
	int isNeedStop() {
		return need_stop_flags.check(get_thread(),false);
	}
	
	void setNeedStop(string name) {
		NamedPool pool = main_pool;
		if(name != "") pool = named_pools.check(name,NULL);
		if(pool == NULL) return;
		forloop(int i = 0; pool.getNumThreads()) {
			if(need_stop_flags.check(pool.getThreadId(i))) need_stop_flags[pool.getThreadId(i)] = true;
		}
	}
}

/*
 */
void asyncInit() {
	AsyncPool::init();
}

void asyncShutdown() {
	AsyncPool::shutdown();
}

void asyncsUpdate() {
	AsyncPool::update();
}

/*
 */
Async asyncGet() {
	return AsyncPool::get();
}

void asyncReleaseAsync(Async async) {
	AsyncPool::releaseAsync(async);
}

/*
 */
int asyncRunThread(int function,int args[] = ()) {
	return AsyncPool::runThread(function,args);
}

int asyncRunNamedThread(int function,string name) {
	return AsyncPool::runThread(function,(),name);
}

int asyncRunNamedThread(int function,int args[],string name) {
	return AsyncPool::runThread(function,args,name);
}

void asyncWaitThreads(string name = "") {
	AsyncPool::waitThreads(name);
}

void asyncClearThreads() {
	AsyncPool::clearThreads();
}

int asyncIsRunning(string name) {
	return AsyncPool::isRunning(name);
}

int asyncIsStopped(string name) {
	return AsyncPool::isStoped(name);
}

void asyncStopThreads(string name) {
	AsyncPool::stop(name);
}

/*
 */
int asyncisNeedStop() {
	return AsyncPool::isNeedStop();
}

void asyncSetNeedStop(string name = "") {
	AsyncPool::setNeedStop(name);
}

#endif /* __EDITOR_ASYNC_H__ */
