package edu.stevens.cs548.clinic.service.web.rest.resources.test;

import java.net.URI;
import java.util.List;

import javax.annotation.PostConstruct;
import javax.ejb.EJB;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.HeaderParam;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.UriBuilder;

import edu.stevens.cs548.clinic.domain.IPatientDAO;
import edu.stevens.cs548.clinic.domain.IProviderDAO;
import edu.stevens.cs548.clinic.domain.Patient;
import edu.stevens.cs548.clinic.domain.PatientDAO;
import edu.stevens.cs548.clinic.domain.Provider;
import edu.stevens.cs548.clinic.domain.ProviderDAO;
import edu.stevens.cs548.clinic.domain.IPatientDAO.PatientException;
import edu.stevens.cs548.clinic.domain.IProviderDAO.ProviderException;
import edu.stevens.cs548.clinic.domain.ITreatmentDAO.TreatmentException;
import edu.stevens.cs548.clinic.service.dto.ProviderDTO;
import edu.stevens.cs548.clinic.service.ejb.IProviderServiceRemote;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderNotFoundException;
import edu.stevens.cs548.clinic.service.ejb.IProviderService.ProviderServiceException;
import edu.stevens.cs548.clinic.service.web.rest.ProviderRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.TreatmentRepresentation;
import edu.stevens.cs548.clinic.service.web.rest.Treatment_PDO_to_Representation_Test;
import edu.stevens.cs548.clinic.service.web.rest.data.DrugTreatmentType;
import edu.stevens.cs548.clinic.service.web.rest.data.RadiologyTreatmentType;
import edu.stevens.cs548.clinic.service.web.rest.data.SurgeryTreatmentType;

@Stateless(name="ProviderResourceTest")
public class ProviderResourceTest {
	
	private IPatientDAO patientDAO;
	private IProviderDAO providerDAO;
	
	public ProviderResourceTest() {
    }
	
	@PersistenceContext(unitName="ClinicDomain")
    private EntityManager em;
	
	@PostConstruct
	private void initialize() {
		patientDAO = new PatientDAO(em);
		providerDAO = new ProviderDAO(em);
	}
	
	@EJB
    private IProviderServiceRemote providerService;
	
    public long addProvider(ProviderRepresentation providerRep) {
    	try {
    		long id = providerService.createProvider(providerRep.getName(), providerRep.getNPI());
    		return id;
    	}
    	catch (ProviderServiceException ex) {
    		throw new WebApplicationException();
    	}
    }
	
    public long addTreatment(String patientDbIdStr, String NPIStr, TreatmentRepresentation treatmentRep) {
    	try {
    		
    		long patientDbId = Long.parseLong(patientDbIdStr);
    		long NPI = Long.parseLong(NPIStr);
    		long tid;
    		
			Patient patient = patientDAO.getPatientByDbId(patientDbId);
			Provider provider = providerDAO.getProviderByNPI(NPI);
			if(provider == null)
				throw new WebApplicationException(Response.Status.NOT_FOUND);
			
			if(treatmentRep.getDrugTreatment() != null) {
				DrugTreatmentType drug = treatmentRep.getDrugTreatment();
				tid = provider.addDrugTreatment(treatmentRep.getDiagnosis(), drug.getDrugName(), drug.getDosage(), patient);
			}
			else if(treatmentRep.getRadiologyTreatment() != null) {
				RadiologyTreatmentType radiology = treatmentRep.getRadiologyTreatment();
				tid = provider.addRadiologyTreatment(treatmentRep.getDiagnosis(), radiology.getDate(), patient);
			}
			else {
				SurgeryTreatmentType surgery = treatmentRep.getSurgeryTreatment();
				tid = provider.addSurgeryTreatment(treatmentRep.getDiagnosis(), surgery.getDate(), patient);
			}
			
			return tid;
			
		}
		catch(PatientException ex) {
			throw new WebApplicationException(Response.Status.NOT_FOUND);
		}
		catch(ProviderException ex) {
			throw new WebApplicationException();
		}
    }
	
    public ProviderRepresentation getProvider(String NPI) throws ProviderNotFoundException, ProviderServiceException {
    	   long npi = Long.parseLong(NPI);
    	   ProviderDTO providerDTO = providerService.getProviderByNPI(npi);
    	   ProviderRepresentation providerRep = new ProviderRepresentation(providerDTO);
    	   return providerRep;
       
    }
	
    public TreatmentRepresentation[] getTreatments(String NPI, String patientDbId) {
       try {
    	    long npi = Long.parseLong(NPI);
    	    long id = Long.parseLong(patientDbId);
			Provider provider = providerDAO.getProviderByNPI(npi);
			Patient patient = patientDAO.getPatientByDbId(id);
			if(provider == null)
				throw new WebApplicationException(Response.Status.NOT_FOUND);
			List<Long> tids = provider.getTreatmentIds(patient);
			TreatmentRepresentation[] treatmentReps = new TreatmentRepresentation[tids.size()];
			for(int i = 0; i < tids.size(); ++i) {
				Treatment_PDO_to_Representation_Test visitor = new Treatment_PDO_to_Representation_Test();
				provider.visitTreatment(tids.get(i), visitor);
				treatmentReps[i] = visitor.getRep();
			}
			return treatmentReps;
		}
		catch(PatientException ex) {
			 throw new WebApplicationException();
		}
		catch(ProviderException ex) {
			 throw new WebApplicationException();
       }
		catch(TreatmentException ex) {
			 throw new WebApplicationException();
		}
    }
}
