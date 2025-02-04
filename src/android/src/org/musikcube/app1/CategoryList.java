/**
 * 
 */
package org.musikcube.app1;

import java.util.ArrayList;

import org.musikcube.core.IQuery;
import org.musikcube.core.ListQuery;
import org.musikcube.core.IQuery.OnQueryResultListener;

import android.app.ListActivity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;

/**
 * @author doy
 *
 */
public class CategoryList extends ListActivity implements OnQueryResultListener {
	
	private String category	= null;
	private String nextCategoryList	= "";
	private ListQuery query	= new ListQuery();
	
	private ArrayList<String> selectedTitles; 
	private ArrayList<String> selectedCategory; 
	private ArrayList<Integer> selectedCategoryIds; 
//	private ProgressDialog loadingDialog;
	
	// Need handler for callbacks to the UI thread
    final Handler callbackHandler = new Handler();

    // Create runnable for posting
    final Runnable callbackRunnable = new Runnable() {
        public void run() {
            OnResults();
        }
    };

    static class CategoryViewHolder{
		TextView title;
	}
    
    public class ResultAdapter extends BaseAdapter{

    	protected ListQuery query;
    	protected ListActivity context;
    	private LayoutInflater inflator;
    	
    	public ResultAdapter(ListActivity context){
    		this.context	= context;
    		this.inflator	= context.getLayoutInflater();
    	}
    	
		public int getCount() {
			int size	= this.query.resultsInts.size();
			if(size==0){
				return 0;
			}else{
				return size+1;
			}
		}

		public Object getItem(int position) {
			if(position==0){
				return 0;
			}else{
				return this.query.resultsInts.get(position-1);
			}
		}

		public long getItemId(int position) {
			if(position==0){
				return 0;
			}else{
				return this.query.resultsInts.get(position-1);
			}
		}

		public View getView(int position, View view, ViewGroup parent) {
			final CategoryViewHolder holder;
			if(view==null){
				view	= inflator.inflate(R.layout.category_list_item, null);
				holder	= new CategoryViewHolder();
				holder.title	= (TextView) view.findViewById(R.id.text); 
				view.setTag(holder);
			}else{
				holder	= (CategoryViewHolder)view.getTag();
			}

			//
			if(position==0){
    			holder.title.setText("- All -");
			}else{
    			holder.title.setText(this.query.resultsStrings.get(position-1));
			}
			return view;
		}
    	
    }
    
    private ResultAdapter listAdapter;
    
	@Override
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		//Log.v("musikcube.CategoryList", "start");

		
		requestWindowFeature(Window.FEATURE_NO_TITLE); 
//		getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE,R.layout.mc2_title); 
//		getWindow().setFe

		this.setContentView(R.layout.category_list);
		
		Intent intent	= this.getIntent();
		
		this.query.SetResultListener(this);
		
		this.listAdapter	= new ResultAdapter(this);
		this.listAdapter.query	= this.query;
		setListAdapter(this.listAdapter);
		

		// Extract the category order
		String categoryString	= intent.getStringExtra("org.musikcube.CategoryList.listCategory");
		String[] categories	= categoryString.split(",");
		this.category	= categories[0];
		
		// Save the next category lists
		for(int i=1;i<categories.length;i++){
			if(i>1){
				this.nextCategoryList	+= ",";
			}
			this.nextCategoryList	+= categories[i];
		}

		this.selectedTitles	= intent.getStringArrayListExtra("org.musikcube.CategoryList.selectedTitles");
		
		if(this.category!=null){
			//Log.v("musikcube.CategoryList", "category="+this.category);
			// Query for data
			this.query.category	= this.category;
			
			// check for selection
			this.selectedCategory 		= intent.getStringArrayListExtra("org.musikcube.CategoryList.selectedCategory");
			this.selectedCategoryIds 	= intent.getIntegerArrayListExtra("org.musikcube.CategoryList.selectedCategoryId");
			if(this.selectedCategory!=null){
				for(int i=0;i<this.selectedCategory.size();i++){
					this.query.SelectData(this.selectedCategory.get(i), this.selectedCategoryIds.get(i));
				}
			}
			
			org.musikcube.core.Library library	= org.musikcube.core.Library.GetInstance();
			
			//this.loadingDialog = ProgressDialog.show(this, "", "Loading "+this.category+"...", true);
            library.AddQuery(this.query);
			
		}else{
			//Log.v("musikcube.CategoryList", "category=null");
			
		}

		// SET TITLES
		TextView headerTitle	= 	(TextView) this.findViewById(R.id.HeaderTitle); 
		headerTitle.setText(this.category);
		
		if(this.selectedTitles!=null){
			TextView headerContent	= 	(TextView) this.findViewById(R.id.HeaderContent); 
			String headerContentString	= "";
			for(int i=0;i<this.selectedTitles.size();i++){
				headerContentString += this.selectedTitles.get(i)+" / ";
			}
			headerContent.setText(headerContentString);
		}
		//Log.v("musikcube.CategoryList", "onCreate end");
		
	}
	
	public void OnResults(){
		//Log.i("CategoryList::OnResults","In right thread "+this.query.resultsStrings.size());
		/*if(this.loadingDialog!=null){
			this.loadingDialog.dismiss();
			this.loadingDialog	= null;
		}*/
		this.listAdapter.notifyDataSetChanged();
	}

	public void OnQueryResults(IQuery query) {
		// Call in right thread
		this.callbackHandler.post(this.callbackRunnable);
	}
	
	@SuppressWarnings("unchecked")
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id){
		//Log.i("CategoryList::onListItemClick","clicked on "+position+" "+id);
		
		// List category
		if(this.selectedTitles==null){
			this.selectedTitles	= new ArrayList<String>();
		}
		if(this.selectedCategory==null){
			this.selectedCategory	= new ArrayList<String>();
		}
		if(this.selectedCategoryIds==null){
			this.selectedCategoryIds	= new ArrayList<Integer>();
		}
		ArrayList<String> selectedTitles	= (ArrayList<String>)this.selectedTitles.clone(); 
		ArrayList<String> selectedCategory	= (ArrayList<String>)this.selectedCategory.clone(); 
		ArrayList<Integer> selectedCategoryIds	= (ArrayList<Integer>)this.selectedCategoryIds.clone(); 
		
		if(position==0){
			selectedTitles.add("-");
		}else{
			selectedTitles.add(this.query.resultsStrings.get(position-1));
		}
		
		if(id!=0){
			selectedCategory.add(this.category);
			selectedCategoryIds.add((int)id);
		}
		
		if(this.nextCategoryList.equals("")){
			// List tracks
			Intent intent	= new Intent(this, TrackList.class);
			intent.putExtra("org.musikcube.CategoryList.listCategory", this.nextCategoryList);
			intent.putExtra("org.musikcube.CategoryList.selectedCategory", selectedCategory);
			intent.putExtra("org.musikcube.CategoryList.selectedCategoryId", selectedCategoryIds);
			intent.putExtra("org.musikcube.CategoryList.selectedTitles", selectedTitles);
			startActivity(intent);
		}else{
			Intent intent	= new Intent(this, CategoryList.class);
			intent.putExtra("org.musikcube.CategoryList.listCategory", this.nextCategoryList);
			intent.putExtra("org.musikcube.CategoryList.selectedCategory", selectedCategory);
			intent.putExtra("org.musikcube.CategoryList.selectedCategoryId", selectedCategoryIds);
			intent.putExtra("org.musikcube.CategoryList.selectedTitles", selectedTitles);
			startActivity(intent);
		}
	}

	@Override
	protected void onPause() {
		super.onPause();
		org.musikcube.core.Library.GetInstance().RemovePointer();
	}

	@Override
	protected void onResume() {
		super.onResume();
		startService(new Intent(this, org.musikcube.app1.Service.class));
		org.musikcube.core.Library.GetInstance().AddPointer();
	}
	
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.default_menu, menu);
        return true;
    }    
    
    public boolean onOptionsItemSelected(MenuItem item) {
    	if(Helper.DefaultOptionsItemSelected(item,this)){
    		return true;
    	}else{
    		return super.onContextItemSelected(item);
    	}
   	}
}
