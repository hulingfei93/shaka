package edu.stevens.cs548.clinic.service.web.soap;

import java.util.Date;

import javax.jws.WebService;
import javax.jws.WebMethod;

import edu.stevens.cs548.clinic.service.dto.PatientDTO;
import edu.stevens.cs548.clinic.service.dto.TreatmentDTO;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.PatientServiceException;
import edu.stevens.cs548.clinic.service.ejb.IPatientService.TreatmentNotFoundException;

@WebService(
		name="IPatientWebPort",
		targetNamespace="http://cs548.stevens.edu/clinic/service/web/soap/patient")
public interface IPatientWebService {
	
	@WebMethod(operationName="create")
	public long createPatient(String name, Date dob, int age, long patientId) throws PatientServiceException; 

	@WebMethod
	public PatientDTO getPatientByDbId(long id) throws PatientServiceException;
	
	@WebMethod
	public PatientDTO getPatientByPatientId(long pid) throws PatientServiceException, PatientNotFoundException;
	
	@WebMethod
	public PatientDTO[] getPatientByNameDob(String name, Date dob);
	
	@WebMethod
	public void deletePatient(String name, long id) throws PatientServiceException;
	
	@WebMethod
	public TreatmentDTO[] getTreatments(long patientId) throws PatientNotFoundException, PatientServiceException;
	
	@WebMethod
	public void deleteTreatment(long id, long tid) throws PatientNotFoundException, TreatmentNotFoundException, PatientServiceException;
	
	@WebMethod
	public String siteInfo();
}
