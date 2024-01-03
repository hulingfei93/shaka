package edu.stevens.cs548.clinic.domain;

import java.util.Date;
import java.util.List;

public interface IPatientDAO {
	
	public static class PatientException extends Exception {
		private static final long serialVersionUID = 1L;
		public PatientException(String msg) {
			super(msg);
		}
	}
	
	public Patient getPatientByPatientId (long pid) throws PatientException;
	
	public Patient getPatientByDbId(long id) throws PatientException;
	
	public List<Patient> getPatientByNameDob(String name, Date dob);
	
	public long addPatient(String name, Date dob, int age) throws PatientException;
	
	public long addPatient(String name, Date dob, int age, long pid) throws PatientException;
	
	public void deletePatient(long id) throws PatientException;
	
}
