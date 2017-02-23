/*
 * Copyright 2013 Gustaf Räntilä
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <q/promise/signal.hpp>

#include <q/mutex.hpp>
#include <q/queue.hpp>

namespace q { namespace detail {

promise_signal::promise_signal( )
: done_( false )
{ }

promise_signal::~promise_signal( )
{ }

// TODO: Analyze noexcept here, when it comes to pushing to a queue which might
//       be closed or similar.
void promise_signal::done( ) noexcept
{
	{
		Q_AUTO_UNIQUE_LOCK( mutex_ );

		done_ = true;
	}

	auto iter = items_.begin( );
	auto end = items_.end( );
	for ( ; iter != end; ++iter )
	{
		auto& item = *iter;

		if ( !item.queue_ ) //item.synchronous_ )
			item.task_( );
		else
			item.queue_->push( std::move( item.task_ ) );
	}

	items_.clear( );
}

void promise_signal::push( task&& task, const queue_ptr& queue ) noexcept
{
	{
		Q_AUTO_UNIQUE_LOCK( mutex_ );

		if ( !done_ )
		{
			items_.push_back(
				{ std::move( task ), queue } );

			return;
		}
	}

	queue->push( std::move( task ) );
}

void promise_signal::push_synchronous( task&& task ) noexcept
{
	{
		Q_AUTO_UNIQUE_LOCK( mutex_ );

		if ( !done_ )
		{
			items_.push_back(
				{ std::move( task ), nullptr } );

			return;
		}
	}

	task( );
}

} } // namespace detail, namespace queue
