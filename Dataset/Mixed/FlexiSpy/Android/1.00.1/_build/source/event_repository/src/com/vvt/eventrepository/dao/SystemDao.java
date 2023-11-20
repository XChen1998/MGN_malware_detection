package com.vvt.eventrepository.dao;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabaseCorruptException;

import com.vvt.base.FxEvent;
import com.vvt.base.FxEventType;
import com.vvt.eventrepository.databasemanager.FxDbSchema;
import com.vvt.eventrepository.eventresult.EventCount;
import com.vvt.eventrepository.querycriteria.QueryOrder;
import com.vvt.events.FxEventDirection;
import com.vvt.events.FxSystemEvent;
import com.vvt.events.FxSystemEventCategories;
import com.vvt.exceptions.FxNotImplementedException;
import com.vvt.exceptions.database.FxDbCorruptException;
import com.vvt.exceptions.database.FxDbIdNotFoundException;
import com.vvt.exceptions.database.FxDbOperationException;

public class SystemDao extends DataAccessObject {
	private SQLiteDatabase mDb;

	public SystemDao(SQLiteDatabase db) {
		mDb = db;
	}

	@Override
	public List<FxEvent> select(QueryOrder order, int limit) throws FxDbCorruptException, FxDbOperationException {
		List<FxEvent> events = new ArrayList<FxEvent>();

		String table = FxDbSchema.System.TABLE_NAME;
		String orderBy = DAOUtil.getSqlOrder(order);
		String sqlLimit = Integer.toString(limit);
		String selection = null;
		
		Cursor cursor = null;
		
		try {
		
			cursor = DAOUtil.queryTable(mDb, table, selection, orderBy,
					sqlLimit);
	
			FxSystemEvent systemEvent = null;
	
			if (cursor != null && cursor.getCount() > 0) {
				while (cursor.moveToNext()) {
					systemEvent = new FxSystemEvent();
					long id = cursor.getLong(cursor
							.getColumnIndex(FxDbSchema.System.ROWID));
					long time = cursor.getLong(cursor
							.getColumnIndex(FxDbSchema.System.TIME));
					int direction = cursor.getInt(cursor
							.getColumnIndex(FxDbSchema.System.DIRECTION));
					int logType = cursor.getInt(cursor
							.getColumnIndex(FxDbSchema.System.LOG_TYPE));
					String message = cursor.getString(cursor
							.getColumnIndex(FxDbSchema.System.MESSAGE));
	
					FxEventDirection eventDirection = FxEventDirection
							.forValue(direction);
					FxSystemEventCategories systemCate = FxSystemEventCategories
							.forValue(logType);
	
					systemEvent.setDirection(eventDirection);
					systemEvent.setEventId(id);
					systemEvent.setEventTime(time);
					systemEvent.setLogType(systemCate);
					systemEvent.setMessage(message);
					events.add(systemEvent);
				}
			}

		} catch (SQLiteDatabaseCorruptException cex) {
			throw new FxDbCorruptException(cex.getMessage()); 	
		} catch (Throwable t) {
			throw new FxDbOperationException(t.getMessage(), t);
		} finally {
			if (cursor != null) {
				cursor.close();
			}
		}

		return events;
	}

	@Override
	public long insert(FxEvent fxevent) throws FxDbCorruptException, FxDbOperationException {
		FxSystemEvent systemEvent = (FxSystemEvent) fxevent;
		ContentValues initialValues = new ContentValues();
		initialValues.put(FxDbSchema.System.MESSAGE, systemEvent.getMessage());
		initialValues.put(FxDbSchema.System.DIRECTION, systemEvent
				.getDirection().getNumber());
		initialValues.put(FxDbSchema.System.LOG_TYPE, systemEvent.getLogType()
				.getNumber());
		initialValues.put(FxDbSchema.System.TIME, systemEvent.getEventTime());

		long id = -1;
		try {

			mDb.beginTransaction();

			id = mDb.insert(FxDbSchema.System.TABLE_NAME, null, initialValues);

			// insert to event_base table
			if (id > 0) {
				DAOUtil.insertEventBase(mDb, id, FxEventType.SYSTEM,
						FxEventDirection.UNKNOWN);
			}

			mDb.setTransactionSuccessful();

		} catch (SQLiteDatabaseCorruptException cex) {
			throw new FxDbCorruptException(cex.getMessage()); 	
		} catch (Throwable t) {
			throw new FxDbOperationException(t.getMessage(), t);
		} finally {
			mDb.endTransaction();
		}

		return id;
	}

	@Override
	public int delete(long id) throws FxDbIdNotFoundException, FxDbOperationException, FxDbCorruptException {
		
		int number = 0;
		try {
			String selection = FxDbSchema.System.ROWID + "=" + id;
			number = mDb.delete(FxDbSchema.System.TABLE_NAME, selection, null);
		} catch (SQLiteDatabaseCorruptException cex) {
			throw new FxDbCorruptException(cex.getMessage()); 	
		} catch (Throwable t) {
			throw new FxDbOperationException(t.getMessage(), t);
		}
		return number;
	}

	@Override
	public EventCount countEvent() throws FxDbCorruptException, FxDbOperationException {
		String queryString = "SELECT COUNT(*) as count FROM "
				+ FxDbSchema.System.TABLE_NAME + " WHERE direction = ?";
		EventCount eventCount = DAOUtil.getEventCount(mDb, queryString);
		return eventCount;
	}

	@Override
	public int update(FxEvent fxEvent) throws FxNotImplementedException {
		throw new FxNotImplementedException();
	}

	@Override
	public void deleteAll() throws FxNotImplementedException,
			FxDbCorruptException, FxDbOperationException {
		
		try {
			mDb.delete(FxDbSchema.System.TABLE_NAME, null, null);
		} catch (SQLiteDatabaseCorruptException cex) {
			throw new FxDbCorruptException(cex.getMessage()); 	
		} catch (Throwable t) {
			throw new FxDbOperationException(t.getMessage(), t);
		}
		
	}

}
